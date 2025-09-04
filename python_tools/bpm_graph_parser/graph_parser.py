# graph_parser.py
import cv2
import numpy as np
import matplotlib.pyplot as plt
import csv
import easyocr

class GraphParser:
    """
    一个用于从图表图像中解析数据点的类。
    通过设置坐标系锚点，将图像中的像素坐标映射为实际数据。
    """
    def __init__(self, image_path: str):
        self.image = cv2.imread(image_path)
        if self.image is None:
            raise FileNotFoundError(f"无法加载图像: {image_path}")
        self.cropped_graph = None
        self.line_mask = None
        self.pixel_points = []
        self.data_points = []
        self.reader = None
        
        self.x_left_px, self.x_right_px, self.y_top_px, self.y_bottom_px = 0, 0, 0, 0
        self.time_start_sec, self.time_end_sec, self.bpm_min_val, self.bpm_max_val = 0, 0, 0, 0
        self.graph_width_px, self.graph_height_px = 0, 0

    def _initialize_ocr_reader(self):
        if self.reader is None:
            print("正在初始化EasyOCR阅读器... (首次运行需要下载模型，请稍候)")
            self.reader = easyocr.Reader(['en'])
            print("EasyOCR阅读器初始化完成。")

    def ocr_read_text(self, crop_box: tuple) -> str:
        self._initialize_ocr_reader()
        x1, y1, x2, y2 = crop_box
        ocr_image_area = self.image[y1:y2, x1:x2]
        gray_image = cv2.cvtColor(ocr_image_area, cv2.COLOR_BGR2GRAY)
        results = self.reader.readtext(gray_image)
        if not results:
            raise ValueError(f"OCR识别失败，未在区域 {crop_box} 中找到任何文本。")
        recognized_text = ' '.join([res[1] for res in results])
        print(f"OCR 识别结果: '{recognized_text}'")
        return recognized_text

    def set_calibration(self, x_coords_px: tuple, y_coords_px: tuple, x_coords_data: tuple, y_coords_data: tuple):
        self.x_left_px, self.x_right_px = x_coords_px
        self.y_top_px, self.y_bottom_px = y_coords_px
        self.time_start_sec, self.time_end_sec = x_coords_data
        self.bpm_min_val, self.bpm_max_val = y_coords_data
        self.graph_width_px = self.x_right_px - self.x_left_px
        self.graph_height_px = self.y_bottom_px - self.y_top_px
        if self.graph_width_px <= 0 or self.graph_height_px <= 0:
            raise ValueError("像素坐标设置错误，宽度或高度小于等于0。")
        print("坐标系校准完成。")

    def extract_line(self, color_ranges: list):
        graph_area = self.image[self.y_top_px:self.y_bottom_px, self.x_left_px:self.x_right_px]
        self.cropped_graph = graph_area
        hsv = cv2.cvtColor(graph_area, cv2.COLOR_BGR2HSV)
        combined_mask = np.zeros(hsv.shape[:2], dtype=np.uint8)
        for lower, upper in color_ranges:
            mask = cv2.inRange(hsv, np.array(lower), np.array(upper))
            combined_mask = cv2.bitwise_or(combined_mask, mask)
        self.line_mask = combined_mask
        print("图表线条提取完成。")

    def parse_pixels_from_line(self):
        if self.line_mask is None: raise ValueError("必须先调用 extract_line() 来提取线条。")
        self.pixel_points = []
        height, width = self.line_mask.shape
        for x in range(width):
            y_coords = np.where(self.line_mask[:, x] > 0)[0]
            if y_coords.size > 0:
                self.pixel_points.append((x, np.mean(y_coords)))
        print(f"从线条中解析出 {len(self.pixel_points)} 个像素点。")

    def convert_pixels_to_data(self):
        if not self.pixel_points: raise ValueError("必须先调用 parse_pixels_from_line() 来解析像素点。")
        self.data_points = []
        total_time_span = self.time_end_sec - self.time_start_sec
        total_bpm_span = self.bpm_max_val - self.bpm_min_val
        for x_px_rel, y_px_rel in self.pixel_points:
            time_sec = self.time_start_sec + (x_px_rel / self.graph_width_px) * total_time_span
            bpm_val = self.bpm_max_val - (y_px_rel / self.graph_height_px) * total_bpm_span
            self.data_points.append((time_sec, bpm_val))
        print("像素点到实际数据的转换完成。")

    def save_to_csv(self, header: list, filename="data.csv"):
        with open(filename, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(header)
            for time_sec, val in self.data_points:
                writer.writerow([f"{time_sec:.2f}", f"{val:.2f}"])
        print(f"数据已成功保存到 {filename}")

    def plot_results(self, data_points: list, title: str, xlabel: str, ylabel: str, save_path=None, color='r'):
        """一个可以绘制任何 (x, y) 数据点列表的通用绘图函数。"""
        if not data_points:
            print("警告：没有可供绘图的数据点。")
            return
            
        time_vals = [p[0] for p in data_points]
        val_vals = [p[1] for p in data_points]
        
        plt.figure(figsize=(10, 5))
        plt.plot(time_vals, val_vals, color=color)
        plt.title(title)
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        plt.grid(True)
        
        if save_path:
            plt.savefig(save_path)
            print(f"图表已保存到: {save_path}")
        else:
            plt.show()
            
        plt.close()

def parse_time_to_seconds(time_str: str) -> float:
    time_str = time_str.strip().replace(' ', '').replace('.', ':')
    try:
        parts = list(map(int, time_str.split(':')))
        if len(parts) == 3: h, m, s = parts; return float(h * 3600 + m * 60 + s)
        elif len(parts) == 2: m, s = parts; return float(m * 60 + s)
        else: raise ValueError("时间格式不是 HH:MM:SS 或 MM:SS")
    except Exception as e:
        raise ValueError(f"无法解析时间字符串 '{time_str}'. 错误: {e}")