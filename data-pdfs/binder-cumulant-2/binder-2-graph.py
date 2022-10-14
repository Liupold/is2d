import glob
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

L_array = [10, 20, 30, 40, 50, 60]
raw_data_dir = "../../raw-data/binder-cumulant-2/"
final_data_dict = {}

for L in L_array:
    files = glob.glob(raw_data_dir + f"binder-cimulant-2_{L**2}_*.txt")
    data = {}; tmp = 0;
    data[f"T [L = {L}]"] = np.loadtxt(files[0], delimiter=',')[:, 0]
    for f in files:
        c = np.loadtxt(f, delimiter=",")
        data["SEED: " + f.split("_")[-1].replace(".txt", "")] = c[:, 1]
        tmp += c

    final_data_dict[L] = tmp / len(files)
    data["Average"] = final_data_dict[L][:, 1];
    pd.DataFrame(data).to_csv(f"TABLE-binder-2_data_L={L}.csv")

fig, ax = plt.subplots(figsize=(11.69,8.27))
for L in L_array:
    plt.plot(*final_data_dict[L].T, '-o', label=f"L = {L}")

Tc = 2 / np.log(1 + np.sqrt(2))
plt.axvline(x=Tc, color='b', label=f'T = {Tc:.5f}')
plt.title("Binder Cumulant 2 (12 seed avg)")
plt.legend()
plt.grid()
plt.savefig("GRAPH-binder-2.pdf")
