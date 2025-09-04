import json

def convert_to_24_hour_format(time_str: str) -> str:
    """
    Converts a time string with Chinese AM/PM markers to ISO 8601 format.
    Example input: "2025/8/31下午10:58"
    Example output: "2025-08-31T22:58:00"
    """
    try:
        is_pm = '下午' in time_str
        is_am = '上午' in time_str

        # Remove the Chinese characters and add a space for splitting
        clean_str = time_str.replace('上午', ' ').replace('下午', ' ')
        
        date_part, time_part = clean_str.split()
        
        # Parse all date and time components to ensure proper zero-padding
        year, month, day = map(int, date_part.split('/'))
        hour, minute = map(int, time_part.split(':'))
        
        # Adjust hour for 24-hour format
        if is_pm and hour < 12:
            hour += 12
        elif is_am and hour == 12:  # Handle midnight case (e.g., 上午12:30 -> 00:30)
            hour = 0
            
        # --- MODIFIED: Reconstruct the string to follow the ISO 8601 standard ---
        # f-string formatting with :02d and :04d handles automatic zero-padding
        return f"{year:04d}-{month:02d}-{day:02d}T{hour:02d}:{minute:02d}:00"

    except (ValueError, IndexError):
        # If OCR result is not in the expected format, return the original string
        print(f"Warning: Could not convert '{time_str}' to 24-hour format. Using original value.")
        return time_str

def sanitize_filename(name: str) -> str:
    """
    Cleans a string to make it a safe filename.
    Replaces characters that are unsafe in Windows/Linux/Mac filenames.
    """
    # Replace slashes, colons, and spaces
    name = name.replace('/', '-').replace(':', '-').replace(' ', '_')
    # You can add more replacement rules here if needed
    return name

def save_dict_to_json(data: dict, filename: str):
    """
    Saves a Python dictionary to a JSON file.

    :param data: The dictionary to be saved.
    :param filename: The path of the output file.
    :raises: Can raise IO-related exceptions if the file cannot be written.
    """
    # The 'with' statement ensures the file is properly closed.
    # encoding='utf-8' is crucial for handling non-ASCII characters.
    # indent=4 makes the JSON file human-readable.
    # ensure_ascii=False allows Chinese characters to be saved correctly.
    with open(filename, 'w', encoding='utf-8') as json_file:
        json.dump(data, json_file, indent=4, ensure_ascii=False)