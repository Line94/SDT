import struct


def int_to_float(file_name):
    with open(file_name, "r") as input_file_buffer:
        # for k in range(1262):
        #     input_file_buffer.write(str(k)+"\n")
        with open("mod_"+file_name, "w") as output_file_buffer:
            data_len = 0
            float_list = []
            while data_len != 10:
                for i in range(1024):
                    input_file = input_file_buffer.readline()
                    output_file_buffer.write(input_file)
                for j in range(238):
                    input_file = input_file_buffer.readline().split("\n")[0]
                    float_buffer = struct.pack('>H', int(input_file))
                    float_list.append(float_buffer)
                    if (j % 2) == 1:
                        float_data = struct.unpack('>f', float_list[0]+float_list[1])
                        output_file_buffer.write(str(float_data) + "\n")
                        float_list.clear()
                data_len += 1


if __name__ == "__main__":
    file_name = "test_data.dat"
    int_to_float(file_name)


