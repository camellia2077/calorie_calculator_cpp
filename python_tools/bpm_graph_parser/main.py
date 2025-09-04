# main.py
import config
import numpy as np
from graph_parser import GraphParser, parse_time_to_seconds
from stats_calculator import calculate_stats

def main():
    """主执行函数"""
    parser = GraphParser(image_path=config.IMAGE_FILE)

    # 1. 使用OCR获取结束时间
    print("\n--- 开始OCR识别结束时间 ---")
    end_time_string = parser.ocr_read_text(crop_box=config.OCR_END_TIME_BOX)
    time_end_sec = parse_time_to_seconds(end_time_string)
    print(f"识别出的总时长: {time_end_sec} 秒")
    print("--- OCR识别完成 ---\n")

    # 2. 根据config中的TARGET_DATA_NAME查找目标数据类型
    target_config = None
    for data_type in config.DATA_TYPES:
        if data_type["name"] == config.TARGET_DATA_NAME:
            target_config = data_type
            break
    
    if not target_config:
        print(f"\n错误: 在 config.py 的 DATA_TYPES 列表中未找到 '{config.TARGET_DATA_NAME}' 的配置。")
        return

    print(f"\n--- 已指定目标数据类型: {target_config['name']} ---")

    # 3. 使用找到的配置处理数据
    x_left, y_top = config.TOP_LEFT_PX
    x_right, y_bottom = config.BOTTOM_RIGHT_PX
    
    # 设置校准参数
    parser.set_calibration(
        x_coords_px=(x_left, x_right),
        y_coords_px=(y_top, y_bottom),
        x_coords_data=(0.0, time_end_sec),
        y_coords_data=(target_config['y_axis_min'], target_config['y_axis_max'])
    )

    # 提取并解析线条
    parser.extract_line(color_ranges=target_config['color_ranges'])
    parser.parse_pixels_from_line()

    # 4. 如果成功提取了数据点，则继续进行进一步处理
    if not parser.pixel_points:
        print(f"\n错误: 根据'{target_config['name']}'的颜色范围，在图像中未找到数据线。")
        return

    print("\n--- 开始处理匹配的数据 ---")
    parser.convert_pixels_to_data()
    
    # 定义CSV表头
    csv_header = ['时间 (秒)', f"{target_config['name']} ({target_config['unit']})"]
    parser.save_to_csv(header=csv_header, filename=config.CSV_OUTPUT_FILE)

    # 计算并打印统计数据
    data_values = [p[1] for p in parser.data_points]
    if data_values:
        # 使用numpy计算平均值和最大值
        avg_val = np.mean(data_values)
        max_val = np.max(data_values)
        
        # 从新模块调用函数计算方差和标准差
        variance, std_dev = calculate_stats(data_values)

        print(f"\n--- {target_config['name']} 分析结果 ---")
        print(f"平均值: {avg_val:.2f} {target_config['unit']}")
        print(f"最大值: {max_val:.2f} {target_config['unit']}")
        
        # 打印新的统计结果
        if variance is not None:
            print(f"方差: {variance:.2f}")
            print(f"标准差: {std_dev:.2f}")

        print("--------------------")

    # 保存原始数据图表
    parser.plot_results(
        data_points=parser.data_points,
        title=f"提取的 {target_config['name']} 数据",
        xlabel="时间 (秒)",
        ylabel=f"{target_config['name']} ({target_config['unit']})",
        save_path=config.PLOT_OUTPUT_FILE
    )

    # --- 5. 计算并绘制斜率 ---
    print("\n--- 开始计算斜率 ---")
    slope_data = []
    if len(parser.data_points) > 1:
        for i in range(1, len(parser.data_points)):
            p1 = parser.data_points[i-1]
            p2 = parser.data_points[i]
            
            time_diff = p2[0] - p1[0]
            value_diff = p2[1] - p1[1]
            
            if time_diff > 0:
                slope = value_diff / time_diff
                slope_data.append((p2[0], slope)) # 将斜率与第二个点的时间关联
    
    if slope_data:
        print(f"已计算 {len(slope_data)} 个斜率点。")
        # 绘制斜率数据
        slope_plot_filename = "slope_plot.png"
        parser.plot_results(
            data_points=slope_data,
            title=f"{target_config['name']} 的斜率",
            xlabel="时间 (秒)",
            ylabel=f"斜率 ({target_config['unit']} / 秒)",
            save_path=slope_plot_filename,
            color='b' # 为斜率图使用不同的颜色
        )
    else:
        print("数据点不足，无法计算斜率。")

if __name__ == '__main__':
    main()