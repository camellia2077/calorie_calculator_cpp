# config.py
# ===================================================================
#                      Graph Parser - Configuration File
# ===================================================================

# --- 1. Basic Configuration ---
# Path to the image file to be analyzed
IMAGE_FILE = r'bpm.jpg'
# Output CSV file name
CSV_OUTPUT_FILE = "extracted_data.csv"
# Output chart image file name
PLOT_OUTPUT_FILE = "extracted_plot.png"


# --- 2. Image Coordinate Configuration (Fixed) ---
# These coordinates are shared by all data types

# Top-left and bottom-right pixel coordinates of the graph grid area (x, y)
TOP_LEFT_PX = (164, 414)
BOTTOM_RIGHT_PX = (2505, 1077)

# OCR area for the end time text (top-left x, top-left y, bottom-right x, bottom-right y)
OCR_END_TIME_BOX = (2374, 1099, 2500, 1130)


# --- 3. Data Type Configuration (Extensible) ---

# MODIFIED: Specify the "name" of the data type you want to recognize
# For example, set it to "Heart Rate" or "Cadence"
TARGET_DATA_NAME = "Heart Rate"

# Define the data types you want to parse in this list.
# The program will look up and use the corresponding configuration based on TARGET_DATA_NAME.
DATA_TYPES = [
    {
        "name": "Heart Rate",                 # Data name
        "unit": "BPM",                      # Data unit
        "y_axis_min": 30.0,                 # Y-axis minimum value
        "y_axis_max": 210.0,                # Y-axis maximum value
        "color_ranges": [                   # HSV color ranges for the line
            # Red color range (includes two parts)
            ([0, 70, 50], [10, 255, 255]),
            ([170, 70, 50], [180, 255, 255]),
        ]
    },
    {
        "name": "Cadence",                   # You can add other types, like Cadence
        "unit": "spm",                      # strides per minute
        "y_axis_min": 0.0,
        "y_axis_max": 220.0,
        "color_ranges": [
             # Example range for yellow
             ([25, 70, 50], [35, 255, 255]),
        ]
    },
    # You can add more data types here...
]