import sys
import os
import config  # Your field configuration
from ocr_extractor import OcrExtractor
from data_saver import save_dict_to_json, sanitize_filename

def process_image(extractor: OcrExtractor, image_path: str):
    """
    Processes a single image: extracts data, validates it, and saves it to JSON.
    
    :param extractor: The initialized OcrExtractor instance.
    :param image_path: The full path to the image file.
    """
    print(f"\n{'='*20} Processing Image: {os.path.basename(image_path)} {'='*20}")
    
    # Part 1: Perform the extraction for the current image.
    extracted_data = extractor.extract_data_from_image(image_path, config.FIELDS) #
    
    # Part 2: Validate the extracted data.
    if "error" in extracted_data:
        print(f"Error processing this image: {extracted_data['error']}")
        return  # Skip to the next image

    start_time_value = extracted_data.get("start_time")
    if not start_time_value:
        print("Error: The critical field 'start_time' could not be extracted or is empty.")
        print("Skipping JSON file generation for this image.")
        return  # Skip to the next image

    # Part 3: Save the data for the current image.
    try:
        output_filename = sanitize_filename(start_time_value) + ".json" #
        save_dict_to_json(extracted_data, output_filename) #
        print(f"Success! Data saved to: {output_filename}")
    except Exception as e:
        print(f"An error occurred while writing the JSON file: {e}")


def main():
    """
    Main function to find all images in a directory and orchestrate the processing.
    """
    # --- Setup: Check directory and find images ---
    image_directory = config.IMAGE_DIRECTORY
    if not os.path.isdir(image_directory):
        print(f"Error: The image directory specified in config.py does not exist: '{image_directory}'")
        sys.exit(1)

    # Define valid image extensions and find all matching files
    valid_extensions = ('.jpg', '.jpeg', '.png', '.bmp', '.tiff')
    image_files = [f for f in os.listdir(image_directory) if f.lower().endswith(valid_extensions)]

    if not image_files:
        print(f"No image files found in '{image_directory}'. Please add images to process.")
        sys.exit(0)
    
    print(f"Found {len(image_files)} image(s) to process in '{image_directory}'.")

    # --- Processing: Initialize model and loop through images ---
    # Create the extractor instance once. The model loads only at the beginning.
    extractor = OcrExtractor()
    
    for filename in image_files:
        full_image_path = os.path.join(image_directory, filename)
        process_image(extractor, full_image_path)
    
    print(f"\n{'='*25} All images processed. {'='*25}")


if __name__ == "__main__":
    main()