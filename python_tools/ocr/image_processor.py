import os
import config
from ocr_extractor import OcrExtractor
from data_saver import DataSaver  # <-- Import the DataSaver CLASS

class ImageProcessor:
    """
    A class to process an image by extracting, transforming, and saving its data.
    """
    def __init__(self, extractor: OcrExtractor):
        """
        Initializes the processor with an OCR extractor instance.
        :param extractor: An initialized instance of OcrExtractor.
        """
        self.extractor = extractor

    def process_image(self, image_path: str, output_directory: str):
        """
        Processes a single image and saves the extracted data to a JSON file.
        :param image_path: The full path to the image file.
        :param output_directory: The directory where the resulting JSON file will be saved.
        """
        print(f"\n{'='*20} Processing Image: {os.path.basename(image_path)} {'='*20}")

        # Part 1: Perform extraction using the stored extractor instance.
        extracted_data = self.extractor.extract_data_from_image(image_path, config.FIELDS)

        # Part 2: Validate the extracted data.
        if "error" in extracted_data:
            print(f"Error processing this image: {extracted_data['error']}")
            return

        if "start_time" not in extracted_data or not extracted_data["start_time"]:
            print("Error: The critical field 'start_time' could not be extracted or is empty.")
            print("Skipping JSON file generation for this image.")
            return

        # --- Data processing using the DataSaver class ---
        final_data = {}
        
        # Use static methods from the DataSaver class for conversion
        start_time_24h = DataSaver.convert_to_24_hour_format(extracted_data.get("start_time", ""))
        unix_timestamp = DataSaver.convert_iso_to_unix(start_time_24h)
        
        final_data["unix_timestamp"] = unix_timestamp
        final_data["raw_data"] = extracted_data

        processed_data_dict = {"start_time_24": start_time_24h}
        
        time_str = final_data["raw_data"].get("time", "")
        time_components = DataSaver.parse_time_to_hms(time_str)
        processed_data_dict.update(time_components)

        raw_sport_type = final_data["raw_data"].get("sport_type")
        if raw_sport_type in config.SPORT_TYPE_MAPPING:
            processed_data_dict["sport_type_en"] = config.SPORT_TYPE_MAPPING[raw_sport_type]

        final_data["processed_data"] = processed_data_dict

        # Part 3: Save the data using the DataSaver class.
        try:
            base_filename = DataSaver.sanitize_filename(start_time_24h) + ".json"
            output_path = os.path.join(output_directory, base_filename)
            DataSaver.save_dict_to_json(final_data, output_path)
            print(f"Success! Data saved to: {output_path}")
        except Exception as e:
            print(f"An error occurred while writing the JSON file: {e}")