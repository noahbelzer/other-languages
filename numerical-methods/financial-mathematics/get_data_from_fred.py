from dotenv import load_dotenv
import os
from fredapi import Fred

load_dotenv()

fred = Fred(api_key=os.getenv("FRED_API_KEY"))
data = fred.get_series("UNRATE", observation_start="2025-01-01")
print(data)
