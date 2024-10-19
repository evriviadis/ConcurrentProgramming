import struct
import random

def write_integers_to_binary_file(file_name, integers):
    with open(file_name, 'wb') as binary_file:
        # Write each integer as binary using struct
        for integer in integers:
            binary_data = struct.pack('i', integer)  # 'i' is for 4-byte integer
            binary_file.write(binary_data)

file_name = 'integers.bin'
integers = []  # List of integers to write
for _ in range(1123):
    integers.append(random.randint(1, 1000))

write_integers_to_binary_file(file_name, integers)
print(f"Written {len(integers)} integers to {file_name}")