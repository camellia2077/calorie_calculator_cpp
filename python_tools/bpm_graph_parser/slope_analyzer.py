# slope_analyzer.py
from __future__ import annotations
import csv
import json
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from graph_parser import GraphParser

def save_data_to_csv(data: list, header: list, filename: str):
    """
    Saves a list of (x, y) tuples to a specified CSV file.
    """
    print(f"正在将斜率数据保存到 {filename}...")
    try:
        with open(filename, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(header)
            for row in data:
                writer.writerow([f"{row[0]:.2f}", f"{row[1]:.2f}"])
        print(f"数据已成功保存到 {filename}")
    except IOError as e:
        print(f"错误：无法写入文件 {filename}。原因: {e}")

def save_data_to_json(data: list, time_key: str, value_key: str, filename: str):
    """
    Saves a list of (x, y) tuples to a specified JSON file.
    """
    print(f"正在将斜率数据保存到 {filename}...")
    output_data = [{time_key: float(f"{row[0]:.2f}"), value_key: float(f"{row[1]:.2f}")} for row in data]
    try:
        with open(filename, 'w', encoding='utf-8') as f:
            json.dump(output_data, f, indent=4, ensure_ascii=False)
        print(f"数据已成功保存到 {filename}")
    except IOError as e:
        print(f"错误：无法写入文件 {filename}。原因: {e}")

def analyze_and_plot_slope(data_points: list, target_config: dict, parser: 'GraphParser', total_duration_sec: float):
    """
    Calculates the slope, saves it to a CSV and JSON, and generates two plots.
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

        csv_filename = "slope_data.csv"
        csv_header = [
            'Time (seconds)',
            f"Slope ({target_config['unit']} / second)"
        ]
        save_data_to_csv(slope_data, csv_header, csv_filename)

        json_filename = f"{int(total_duration_sec)}_slope.json"
        save_data_to_json(slope_data, time_key='time_sec', value_key='slope', filename=json_filename)

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

        scatter_plot_filename = "slope_scatter_plot.png"
        print(f"正在生成斜率散点图: {scatter_plot_filename}")
        
        slope_values = [p[1] for p in slope_data]
        scatter_colors = ['#d62728' if v > 0 else '#1f77b4' for v in slope_values]

        parser.plot_results(
            data_points=slope_data,
            title=f"Rate of Change for {target_config['name']} (Scatter Plot)",
            xlabel="Time (seconds)",
            ylabel=f"Change Rate ({target_config['unit']} / second)",
            save_path=scatter_plot_filename,
            color=scatter_colors,
            plot_style='scatter'
        )
    else:
        print("数据点不足，无法计算斜率。")