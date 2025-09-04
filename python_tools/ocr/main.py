import sys
import os
import config
from ocr_extractor import OcrExtractor
from image_processor import ImageProcessor  # <-- IMPORT the ImageProcessor CLASS

def validate_config():
    """
    Checks that all required directory paths in the config file are set.
    """
    print("--- Validating configuration ---")
    if not config.IMAGE_DIRECTORY:
        print("Error: 'IMAGE_DIRECTORY' in config.py is empty. Please specify the path to your images.")
        sys.exit(1)
    if not config.JSON_OUTPUT_DIRECTORY:
        print("Error: 'JSON_OUTPUT_DIRECTORY' in config.py is empty. Please specify a path for JSON output.")
        sys.exit(1)
    print("Configuration is valid.")

def main():
    """
    Main function to find all images in a directory and orchestrate the processing.
    """
    validate_config()

    image_directory = config.IMAGE_DIRECTORY
    output_directory = config.JSON_OUTPUT_DIRECTORY

    if not os.path.isdir(image_directory):
        print(f"Error: The image directory specified does not exist: '{image_directory}'")
        sys.exit(1)
        
    if not os.path.isdir(output_directory):
        print(f"Output directory '{output_directory}' not found. Creating it now.")
        os.makedirs(output_directory)

    valid_extensions = ('.jpg', '.jpeg', '.png', '.bmp', '.tiff')
    image_files = [f for f in os.listdir(image_directory) if f.lower().endswith(valid_extensions)]

    if not image_files:
        print(f"No image files found in '{image_directory}'. Please add images to process.")
        sys.exit(0)
    
    print(f"Found {len(image_files)} image(s) to process in '{image_directory}'.")

    # --- Instantiate the classes ---
    extractor = OcrExtractor()
    # Create an instance of the processor, passing the extractor to it.
    processor = ImageProcessor(extractor)
    
    for filename in image_files:
        full_image_path = os.path.join(image_directory, filename)
        # Call the process_image METHOD on the processor instance.
        processor.process_image(full_image_path, output_directory)
    
    print(f"\n{'='*25} All images processed. {'='*25}")

if __name__ == "__main__":
    main()