import json
from datetime import datetime

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
        elif is_am and hour == 12:
            hour = 0
            
        return f"{year:04d}-{month:02d}-{day:02d}T{hour:02d}:{minute:02d}:00"

    except (ValueError, IndexError):
        print(f"Warning: Could not convert '{time_str}' to 24-hour format. Using original value.")
        return time_str

def convert_iso_to_unix(iso_str: str) -> int:
    """
    Converts an ISO 8601 formatted string to a Unix timestamp (integer seconds).
    
    :param iso_str: A time string in "YYYY-MM-DDTHH:MM:SS" format.
    :return: An integer representing the Unix timestamp, or 0 if conversion fails.
    """
    try:
        # Python 3.7+ can directly parse this ISO format
        dt_object = datetime.fromisoformat(iso_str)
        # .timestamp() returns a float, so we convert it to an integer
        return int(dt_object.timestamp())
    except (ValueError, TypeError):
        # Handle cases where the iso_str is not in the correct format
        print(f"Warning: Could not convert '{iso_str}' to a Unix timestamp. Defaulting to 0.")
        return 0

def parse_time_to_hms(time_str: str) -> dict:
    """
    Parses a time string like "HH:MM:SS" into a dictionary of integers.
    If the format is incorrect, it returns a dictionary with zero values.
    
    :param time_str: The time string from OCR, e.g., "00:19:08".
    :return: A dictionary like {"hours": 0, "minutes": 19, "seconds": 8}.
    """
    try:
        parts = list(map(int, time_str.split(':')))
        
        if len(parts) == 3:
            return {"hours": parts[0], "minutes": parts[1], "seconds": parts[2]}
        elif len(parts) == 2:
            return {"hours": 0, "minutes": parts[0], "seconds": parts[1]}
        else:
            print(f"Warning: Unexpected time format '{time_str}'. Defaulting to 0.")
            return {"hours": 0, "minutes": 0, "seconds": 0}

    except (ValueError, IndexError, AttributeError):
        print(f"Warning: Could not parse time string '{time_str}'. Defaulting to 0.")
        return {"hours": 0, "minutes": 0, "seconds": 0}

def sanitize_filename(name: str) -> str:
    """
    Cleans a string to make it a safe filename.
    Replaces characters that are unsafe in Windows/Linux/Mac filenames.
    """
    name = name.replace('/', '-').replace(':', '-').replace(' ', '_')
    return name

def save_dict_to_json(data: dict, filename: str):
    """
    Saves a Python dictionary to a JSON file.

    :param data: The dictionary to be saved.
    :param filename: The path of the output file.
    :raises: Can raise IO-related exceptions if the file cannot be written.
    """
    with open(filename, 'w', encoding='utf-8') as json_file:
        json.dump(data, json_file, indent=4, ensure_ascii=False)