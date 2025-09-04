import sys
import os
import config  # Your field configuration
from ocr_extractor import OcrExtractor
# Import the helper functions from data_saver
from data_saver import (
    save_dict_to_json, 
    sanitize_filename, 
    convert_to_24_hour_format, 
    parse_time_to_hms,
    convert_iso_to_unix
)

def validate_config():
    """
    Checks that all required directory paths in the config file are set.
    If any path is empty or None, the program will exit.
    """
    print("--- Validating configuration ---")
    
    # Check if the image directory path is provided
    if not config.IMAGE_DIRECTORY:
        print("Error: 'IMAGE_DIRECTORY' in config.py is empty. Please specify the path to your images.")
        sys.exit(1)
        
    # Check if the JSON output directory path is provided
    if not config.JSON_OUTPUT_DIRECTORY:
        print("Error: 'JSON_OUTPUT_DIRECTORY' in config.py is empty. Please specify a path for JSON output.")
        sys.exit(1)
        
    print("Configuration is valid.")

def process_image(extractor: OcrExtractor, image_path: str):
    """
    Processes a single image: extracts data, validates it, and saves it to JSON.
    
    :param extractor: The initialized OcrExtractor instance.
    :param image_path: The full path to the image file.
    """
    print(f"\n{'='*20} Processing Image: {os.path.basename(image_path)} {'='*20}")
    
    # Part 1: Perform the extraction for the current image.
    extracted_data = extractor.extract_data_from_image(image_path, config.FIELDS)
    
    # Part 2: Validate the extracted data.
    if "error" in extracted_data:
        print(f"Error processing this image: {extracted_data['error']}")
        return  # Skip to the next image

    if "start_time" not in extracted_data or not extracted_data["start_time"]:
        print("Error: The critical field 'start_time' could not be extracted or is empty.")
        print("Skipping JSON file generation for this image.")
        return  # Skip to the next image

    # --- MODIFIED: Elevate Unix timestamp to the top level ---
    
    # Initialize the final dictionary that will be saved to JSON.
    final_data = {}
    
    # Generate the 24-hour timestamp first, as it's needed for the Unix timestamp.
    start_time_24h = convert_to_24_hour_format(extracted_data.get("start_time", ""))

    # --- NEW: Calculate and add the Unix timestamp to the TOP-LEVEL dictionary ---
    unix_timestamp = convert_iso_to_unix(start_time_24h)
    final_data["unix_timestamp"] = unix_timestamp

    # Place all the raw OCR results under the "raw_data" key.
    final_data["raw_data"] = extracted_data
    
    # Now, build the "processed_data" dictionary.
    processed_data_dict = {}

    # Add the 24-hour timestamp.
    processed_data_dict["start_time_24"] = start_time_24h

    # Parse and add the time components (h, m, s).
    time_str = final_data["raw_data"].get("time", "")
    time_components = parse_time_to_hms(time_str)
    processed_data_dict.update(time_components)

    # Add the completed "processed_data" dictionary to the main data object.
    final_data["processed_data"] = processed_data_dict
    # --- END OF MODIFICATION ---

    # Part 3: Save the data for the current image.
    try:
        # Create a safe base filename from the ISO 8601 timestamp.
        base_filename = sanitize_filename(start_time_24h) + ".json"
        
        # Combine the output directory with the base filename to get the full path.
        output_path = os.path.join(config.JSON_OUTPUT_DIRECTORY, base_filename)
        
        # Save the newly structured dictionary to the specified path.
        save_dict_to_json(final_data, output_path)
        print(f"Success! Data saved to: {output_path}")
    except Exception as e:
        print(f"An error occurred while writing the JSON file: {e}")


def main():
    """
    Main function to find all images in a directory and orchestrate the processing.
    """
    # --- Step 1: Validate Configuration ---
    validate_config()

    # --- Step 2: Setup Directories ---
    image_directory = config.IMAGE_DIRECTORY
    output_directory = config.JSON_OUTPUT_DIRECTORY

    # Check if image directory exists
    if not os.path.isdir(image_directory):
        print(f"Error: The image directory specified in config.py does not exist: '{image_directory}'")
        sys.exit(1)
        
    # Check if output directory exists, create it if it doesn't
    if not os.path.isdir(output_directory):
        print(f"Output directory '{output_directory}' not found. Creating it now.")
        os.makedirs(output_directory)

    # --- Step 3: Find and Process Images ---
    # Define valid image extensions and find all matching files
    valid_extensions = ('.jpg', '.jpeg', '.png', '.bmp', '.tiff')
    image_files = [f for f in os.listdir(image_directory) if f.lower().endswith(valid_extensions)]

    if not image_files:
        print(f"No image files found in '{image_directory}'. Please add images to process.")
        sys.exit(0)
    
    print(f"Found {len(image_files)} image(s) to process in '{image_directory}'.")

    # Create the extractor instance once. The model loads only at the beginning.
    extractor = OcrExtractor()
    
    for filename in image_files:
        full_image_path = os.path.join(image_directory, filename)
        process_image(extractor, full_image_path)
    
    print(f"\n{'='*25} All images processed. {'='*25}")


if __name__ == "__main__":
    main()