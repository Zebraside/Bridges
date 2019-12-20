import matplotlib.pyplot as plt
import pandas as pd

headers = ['Vertex count','Average Edge count','Edge probability', 'One bridge determined time', 'One bridge Random time', 'Two bridge random time']

df = pd.read_csv('/home/kmolchan/Private/Bridges/radix_stack_64.txt',names=headers)
x = df['Average Edge count'][:10]
xx = df['Vertex count'][:10]
y = df['Two bridge random time'][:10]
plt.plot(xx,y, marker='o')

def show_plot(filename):
    df = pd.read_csv(filename,names=headers)
    x = df['Average Edge count'][:10]
    xx = df['Vertex count'][:10]
    y = df['Two bridge random time'][:10]
    plt.plot(xx,y, marker='o')
show_plot('/home/kmolchan/Private/Bridges/radix_stack_64.txt')

plt.show()
