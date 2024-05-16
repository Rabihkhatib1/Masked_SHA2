import os
import random

# Path to the output folder and file
output_folder = 'bin'
output_file = os.path.join(output_folder, 'rand_gen')

# Ensure the output folder exists
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Generate 6592 random bytes
random_data = random.getrandbits(6592 * 8).to_bytes(6592, 'big')

# Write the random data to the file
with open(output_file, 'wb') as file:
    file.write(random_data)

print(f"Random data generated and stored in {output_file}")
