import json
from datetime import datetime

class DataSaver:
    """
    A class that provides static methods for data conversion and file operations.
    """

    @staticmethod
    def convert_to_24_hour_format(time_str: str) -> str:
        """
        Converts a time string with Chinese AM/PM markers to ISO 8601 format.
        Example input: "2025/8/31下午10:58"
        Example output: "2025-08-31T22:58:00"
        """
        try:
            is_pm = '下午' in time_str
            is_am = '上午' in time_str
            clean_str = time_str.replace('上午', ' ').replace('下午', ' ')
            date_part, time_part = clean_str.split()
            year, month, day = map(int, date_part.split('/'))
            hour, minute = map(int, time_part.split(':'))
            if is_pm and hour < 12:
                hour += 12
            elif is_am and hour == 12:
                hour = 0
            return f"{year:04d}-{month:02d}-{day:02d}T{hour:02d}:{minute:02d}:00"
        except (ValueError, IndexError):
            print(f"Warning: Could not convert '{time_str}' to 24-hour format. Using original value.")
            return time_str

    @staticmethod
    def convert_iso_to_unix(iso_str: str) -> int:
        """
        Converts an ISO 8601 formatted string to a Unix timestamp (integer seconds).
        """
        try:
            dt_object = datetime.fromisoformat(iso_str)
            return int(dt_object.timestamp())
        except (ValueError, TypeError):
            print(f"Warning: Could not convert '{iso_str}' to a Unix timestamp. Defaulting to 0.")
            return 0

    @staticmethod
    def parse_time_to_hms(time_str: str) -> dict:
        """
        Parses a time string like "HH:MM:SS" into a dictionary of integers.
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

    @staticmethod
    def sanitize_filename(name: str) -> str:
        """
        Cleans a string to make it a safe filename.
        """
        return name.replace('/', '-').replace(':', '-').replace(' ', '_')

    @staticmethod
    def save_dict_to_json(data: dict, filename: str):
        """
        Saves a Python dictionary to a JSON file.
        """
        with open(filename, 'w', encoding='utf-8') as json_file:
            json.dump(data, json_file, indent=4, ensure_ascii=False)