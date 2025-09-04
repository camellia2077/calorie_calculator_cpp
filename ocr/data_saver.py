import json

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