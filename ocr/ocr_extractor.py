import easyocr
from PIL import Image
import numpy as np
import torch

class OcrExtractor:
    """
    A class dedicated to extracting text from specified regions of an image.
    The model is initialized only once for efficiency.
    """
    def __init__(self):
        """
        Initializes the OcrExtractor, loading the EasyOCR model.
        It auto-detects and uses a GPU if available.
        """
        use_gpu = torch.cuda.is_available()
        if use_gpu:
            print(f"NVIDIA GPU detected: {torch.cuda.get_device_name(0)}. Using GPU acceleration.")
        else:
            print("Warning: No available NVIDIA GPU detected. Running on CPU.")
        
        # The model is loaded once during initialization.
        self.reader = easyocr.Reader(['ch_sim', 'en'], gpu=use_gpu)
        print("Model loaded, extractor is ready.")

    def extract_data_from_image(self, image_path: str, fields: list) -> dict:
        """
        Extracts data from an image based on a list of fields and their coordinates.

        :param image_path: Path to the image file.
        :param fields: A list of field dictionaries loaded from the config file.
        :return: A Python dictionary containing the extracted results.
        """
        results_dict = {}
        try:
            source_image = Image.open(image_path)
            # The line below has been commented out.
            # print("Starting data extraction for the following fields:")
            for field in fields:
                field_name = field['name']
                box = field['box']
                
                # Crop the image and convert to a NumPy array for EasyOCR
                cropped_image_pil = source_image.crop(box)
                cropped_image_np = np.array(cropped_image_pil)
                
                # Perform OCR on the cropped region
                ocr_results = self.reader.readtext(cropped_image_np)
                
                # Join results and clean up whitespace
                extracted_text = ' '.join([res[1] for res in ocr_results]).strip()
                results_dict[field_name] = extracted_text
            
            return results_dict

        except FileNotFoundError:
            # Handle specific error for file not found
            return {"error": f"Image file not found: {image_path}"}
        except Exception as e:
            # Handle other potential errors during processing
            return {"error": f"An error occurred during processing: {str(e)}"}