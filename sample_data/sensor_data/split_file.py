import csv
import os

def split_csv(input_file, max_rows):
    for file_num in range(52):
        with open(input_file, 'r') as infile:
            reader = csv.reader(infile)
            header = next(reader)  # Skip header if there is one

            sensor_num = str(file_num)
            if (file_num < 10):
                sensor_num = "0" + sensor_num

            output_dir = f'sensor_{sensor_num}'
            os.makedirs(output_dir, exist_ok=True)

            rows = []
            for row in reader:
                # Keep only the first two values
                rows.append([row[0], row[file_num+2]])
            
            file_count = 1
            for i in range(0, len(rows), max_rows):
                if file_count > 20:
                    break
                    
                output_file = f'sensor_{sensor_num}/output_{file_count}.csv'
                with open(output_file, 'w', newline='') as outfile:
                    writer = csv.writer(outfile)
                    writer.writerow(['index', f'sensor{sensor_num}'])  # Write header if needed
                    writer.writerows(rows[i:i+max_rows])
                file_count += 1

# Usage
split_csv('sensor.csv', 1000)
