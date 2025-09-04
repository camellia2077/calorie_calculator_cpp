# slope_analyzer.py

# Used for type hinting to avoid circular import errors with graph_parser
from __future__ import annotations
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from graph_parser import GraphParser

def analyze_and_plot_slope(data_points: list, target_config: dict, parser: 'GraphParser'):
    """
    Calculates the slope from data points and generates two plots:
    1. A color-coded bar chart.
    2. A color-coded scatter plot with a zero-line reference.
    """
    print("\n--- 开始分析和绘制斜率图 ---")
    slope_data = []
    if len(data_points) > 1:
        for i in range(1, len(data_points)):
            p1 = data_points[i-1]
            p2 = data_points[i]
            time_diff = p2[0] - p1[0]
            value_diff = p2[1] - p1[1]
            if time_diff > 0:
                slope = value_diff / time_diff
                slope_data.append((p2[0], slope))
    
    if slope_data:
        print(f"已计算 {len(slope_data)} 个斜率点。")

        # --- 生成图表 1: 彩色条形图 (Color-Coded Bars) ---
        bar_chart_filename = "slope_bar_chart.png"
        print(f"正在生成斜率条形图: {bar_chart_filename}")
        parser.plot_results(
            data_points=slope_data,
            title=f"Rate of Change for {target_config['name']} (Bar Chart)",
            xlabel="Time (seconds)",
            ylabel=f"Change Rate ({target_config['unit']} / second)",
            save_path=bar_chart_filename,
            plot_style='bar'
        )

        # --- 生成图表 2: 带参考线的散点图 (Zero-Line Reference Scatter) ---
        scatter_plot_filename = "slope_scatter_plot.png"
        print(f"正在生成斜率散点图: {scatter_plot_filename}")
        
        # 为散点图创建颜色列表 (红色代表正斜率, 蓝色代表负斜率)
        slope_values = [p[1] for p in slope_data]
        scatter_colors = ['#d62728' if v > 0 else '#1f77b4' for v in slope_values]

        parser.plot_results(
            data_points=slope_data,
            title=f"Rate of Change for {target_config['name']} (Scatter Plot)",
            xlabel="Time (seconds)",
            ylabel=f"Change Rate ({target_config['unit']} / second)",
            save_path=scatter_plot_filename,
            color=scatter_colors,  # 传递颜色列表
            plot_style='scatter'
        )
    else:
        print("数据点不足，无法计算斜率。")