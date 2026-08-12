# =============================================================================
# BLS Web Scraper
# =============================================================================
# DESCRIPTION:
#   Scrapes a data table from a BLS (Bureau of Labor Statistics) webpage and
#   saves it to a CSV file. On subsequent runs, it updates existing values
#   and appends any new rows (e.g. a new year of data).
#
# REQUIREMENTS:
#   pip install pandas requests
#
# USAGE:
#   1. Run the script directly to use the default URL and output file:
#        python3 bls_scraper.py
#
#   2. Or call the function from another script:
#        from bls_scraper import bls_scraper
#        bls_scraper("output.csv", "https://data.bls.gov/timeseries/CUUR0000SA0&output_view=pct_12mths")
#
#   3. On the FIRST run, no CSV file is needed. One will be created automatically.
#      On subsequent runs, the existing CSV will be updated with any new/revised data.
#    
#   4. To change the URL or the name of the output csv file, change the line in the main function.
#      In other words, replace the file name and URL in this part at the bottom of the code:
#      if __name__ == "__main__":
#          bls_scraper("output.csv", "https://data.bls.gov/timeseries/CUUR0000SA0&output_view=pct_12mths")
#
# OUTPUT:
#   A CSV file (default: output.csv) containing the scraped BLS table data.
# =============================================================================

import pandas as pd
import requests
import re
import os


def bls_scraper(file, url):

    # --- Fetch HTML content from the BLS URL ---
    response = requests.get(url)
    html_content = response.text

    # --- Read old data if the file already exists ---
    if os.path.exists(file):
        old_data = pd.read_csv(file)
        print("OLD Data:")
        print(old_data)
    else:
        old_data = None
        print("No existing file found. A new one will be created.")

    # --- Extract the table with id="table0" ---
    table_match = re.search(r'<TABLE id="table0".*?</TABLE>', html_content, re.DOTALL | re.IGNORECASE)
    if not table_match:
        print("Error: Could not find table with id='table0' in the page.")
        return
    table_content = table_match.group(0)

    # --- Extract headers from <THEAD> ---
    headers_match = re.search(r'<THEAD>.*?<TR.*?>(.*?)</TR>.*?</THEAD>', table_content, re.DOTALL | re.IGNORECASE)
    if not headers_match:
        print("Error: Could not find table headers.")
        return
    header_cells = re.findall(r'<TH.*?>(.*?)</TH>', headers_match.group(1), re.DOTALL | re.IGNORECASE)
    header_cells = [h.strip() for h in header_cells]

    # --- Extract table body rows ---
    tbody_match = re.search(r'<TBODY.*?>(.*?)</TBODY>', table_content, re.DOTALL | re.IGNORECASE)
    if not tbody_match:
        print("Error: Could not find table body.")
        return
    rows = re.findall(r'<TR.*?>(.*?)</TR>', tbody_match.group(1), re.DOTALL | re.IGNORECASE)

    # --- Parse each row ---
    data = []
    for row in rows:
        year_match = re.findall(r'<TH.*?>(.*?)</TH>', row, re.DOTALL | re.IGNORECASE)
        data_cells = re.findall(r'<TD.*?>(.*?)</TD>', row, re.DOTALL | re.IGNORECASE)
        if year_match:
            row_data = [year_match[0].strip()] + [cell.strip() for cell in data_cells]
            data.append(row_data)

    # --- Build DataFrame and convert numeric columns ---
    new_data = pd.DataFrame(data, columns=header_cells)
    for col in new_data.columns[1:]:
        new_data[col] = pd.to_numeric(new_data[col], errors='coerce')
    new_data[new_data.columns[0]] = pd.to_numeric(new_data[new_data.columns[0]], errors='coerce')

    print("NEW Data:")
    print(new_data)

    # --- First run: no existing file, just save and exit ---
    if old_data is None:
        new_data.to_csv(file, index=False)
        print(f"Done - New file created: {file}")
        return

    # --- Append new rows if a new year exists in the scraped data ---
    last_year_old = old_data.iloc[-1, 0]
    last_year_new = new_data.iloc[-1, 0]

    if last_year_new > last_year_old:
        new_rows = new_data[new_data.iloc[:, 0] > last_year_old]
        new_rows.to_csv(file, mode='a', header=False, index=False)
        print(f"{len(new_rows)} new row(s) were added.")
        old_data = pd.read_csv(file)

    # --- Update individual values in old_data from new_data ---
    first_new_year = new_data.iloc[0, 0]

    for i in range(len(old_data)):
        if old_data.iloc[i, 0] == first_new_year:
            for j in range(min(11, len(new_data))):
                for k in range(min(15, len(old_data.columns))):
                    old_val = old_data.iloc[i + j, k]
                    new_val = new_data.iloc[j, k]
                    if old_val != new_val:
                        old_data.iloc[i + j, k] = new_val
                        print(f"Value updated: row {i + j}, col {k} -> {new_val}")
            break

    # --- Write updated data back to file ---
    old_data.to_csv(file, index=False)
    print("Done - Data updated.")


if __name__ == "__main__":
    bls_scraper("output.csv", "https://data.bls.gov/timeseries/CUUR0000SA0&output_view=pct_12mths")
