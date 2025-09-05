# main.py
import config
import numpy as np
from graph_parser import GraphParser, parse_time_to_seconds
from stats_calculator import calculate_stats
from slope_analyzer import analyze_and_plot_slope

def main():
    """主执行函数"""
    parser = GraphParser(image_path=config.IMAGE_FILE)

    print("\n--- 开始OCR识别结束时间 ---")
    end_time_string = parser.ocr_read_text(crop_box=config.OCR_END_TIME_BOX)
    time_end_sec = parse_time_to_seconds(end_time_string)
    print(f"识别出的总时长: {time_end_sec} 秒")
    print("--- OCR识别完成 ---\n")

    target_config = None
    for data_type in config.DATA_TYPES:
        if data_type["name"] == config.TARGET_DATA_NAME:
            target_config = data_type
            break
    
    if not target_config:
        print(f"\n错误: 在 config.py 的 DATA_TYPES 列表中未找到 '{config.TARGET_DATA_NAME}' 的配置。")
        return

    print(f"\n--- 已指定目标数据类型: {target_config['name']} ---")

    x_left, y_top = config.TOP_LEFT_PX
    x_right, y_bottom = config.BOTTOM_RIGHT_PX
    
    parser.set_calibration(
        x_coords_px=(x_left, x_right),
        y_coords_px=(y_top, y_bottom),
        x_coords_data=(0.0, time_end_sec),
        y_coords_data=(target_config['y_axis_min'], target_config['y_axis_max'])
    )

    parser.extract_line(color_ranges=target_config['color_ranges'])
    parser.parse_pixels_from_line()

    if not parser.pixel_points:
        print(f"\n错误: 根据'{target_config['name']}'的颜色范围，在图像中未找到数据线。")
        return

    print("\n--- 开始处理匹配的数据 ---")
    parser.convert_pixels_to_data()
    
    csv_header = ['时间 (秒)', f"{target_config['name']} ({target_config['unit']})"]
    parser.save_to_csv(header=csv_header, filename=config.CSV_OUTPUT_FILE)

    json_filename = f"{int(time_end_sec)}_bpm.json"
    parser.save_to_json(
        data_points=parser.data_points,
        filename=json_filename,
        time_key='time_sec',
        value_key='bpm'
    )

    data_values = [p[1] for p in parser.data_points]
    if data_values:
        avg_val = np.mean(data_values)
        max_val = np.max(data_values)
        variance, std_dev = calculate_stats(data_values)

        print(f"\n--- {target_config['name']} 分析结果 ---")
        print(f"平均值: {avg_val:.2f} {target_config['unit']}")
        print(f"最大值: {max_val:.2f} {target_config['unit']}")
        if variance is not None:
            print(f"方差: {variance:.2f}")
            print(f"标准差: {std_dev:.2f}")
        print("--------------------")

    parser.plot_results(
        data_points=parser.data_points,
        title=f"Extracted {target_config['name']} Data",
        xlabel="Time (seconds)",
        ylabel=f"{target_config['name']} ({target_config['unit']})",
        save_path=config.PLOT_OUTPUT_FILE
    )

    analyze_and_plot_slope(parser.data_points, target_config, parser, time_end_sec)

if __name__ == '__main__':
    main()