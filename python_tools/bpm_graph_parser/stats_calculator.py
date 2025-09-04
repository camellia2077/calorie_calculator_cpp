# stats_calculator.py

import numpy as np

def calculate_stats(data_values: list):
    """
    Calculates the variance and standard deviation of a dataset.

    Parameters:
    data_values (list): A list containing numerical values.

    Returns:
    tuple: (variance, standard_deviation). Returns (None, None) if the list is empty.
    """
    if not data_values:
        return None, None
    
    variance = np.var(data_values)
    std_dev = np.std(data_values)
    
    return variance, std_dev