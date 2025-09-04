# slope_analyzer.py

# Used for type hinting to avoid circular import errors with graph_parser
from __future__ import annotations
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from graph_parser import GraphParser

def analyze_and_plot_slope(data_points: list, target_config: dict, parser: 'GraphParser'):
    """
    Calculates the slope from data points and generates a scatter plot.

    Parameters:
    data_points (list): A list of (time, value) tuples from the main parser.
    target_config (dict): The configuration dictionary for the target data type.
    parser (GraphParser): The GraphParser instance to use its plot_results method.
    """
    print("\n--- 开始计算斜率 ---")
    slope_data = []
    if len(data_points) > 1:
        # Iterate through points to calculate the slope of each segment
        for i in range(1, len(data_points)):
            p1 = data_points[i-1]
            p2 = data_points[i]
            
            time_diff = p2[0] - p1[0]
            value_diff = p2[1] - p1[1]
            
            if time_diff > 0:
                slope = value_diff / time_diff
                slope_data.append((p2[0], slope)) # Associate slope with the second point's time
    
    if slope_data:
        print(f"已计算 {len(slope_data)} 个斜率点。")
        # Call the plotting function with the new 'scatter' style
        slope_plot_filename = "slope_plot.png"
        parser.plot_results(
            data_points=slope_data,
            title=f"Slope of {target_config['name']}",
            xlabel="Time (seconds)",
            ylabel=f"Slope ({target_config['unit']} / second)",
            save_path=slope_plot_filename,
            color='b',
            plot_style='scatter'  # This now creates a scatter plot with dots
        )
    else:
        print("数据点不足，无法计算斜率。")