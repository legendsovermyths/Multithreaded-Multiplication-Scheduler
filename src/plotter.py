import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


def plot_graph(df_path, title, xlabel, ylabel, xdata, ydata, hue, img_path):
    df = pd.read_csv(df_path)
    plt.rcParams.update({'font.size': 16})
    fig, ax = plt.subplots(figsize=(12, 12), dpi=120)
    palette = sns.color_palette("mako_r", 6)
    sns.lineplot(x=xdata, y=ydata,
                 data=df,
                 hue=hue,
                 palette=palette,
                 ax=ax)
    ax.set(xlabel=xlabel,
           ylabel=ylabel,
           title=title)
    plt.savefig(img_path)
    return


plot_graph(df_path=r'../csvfiles/multithreading_multiplication.csv',
           title="Performance (Multiplication) vs. Size of Matrix",
           xlabel="Size of Matrix",
           ylabel="Time of Execution (ns)",
           xdata='Size',
           ydata='Time',
           hue='Threads',
           img_path=r'../images/multiplication.jpg')

plot_graph(df_path=r'../csvfiles/multithreading_reading.csv',
           title="Performance (Reading) vs. Size of Matrix",
           xlabel="Size of Matrix",
           ylabel="Time of Execution (ns)",
           xdata='Size',
           ydata='Time',
           hue='Threads',
           img_path='../images/reading.jpg')

plot_graph(df_path=r'../csvfiles/turnaroundTime_1ms.csv',
           title="Turnaround Time (quanta = 1ms) vs. Size of Matrix",
           xlabel="Size of Matrix",
           ylabel="Time of Execution (ns)",
           xdata='Size',
           ydata='Time',
           hue=None,
           img_path=r'../images/turnaroundTime1ms.jpg')

plot_graph(df_path=r'../csvfiles/turnaroundTime_2ms.csv',
           title="Turnaround Time (quanta = 2ms) vs. Size of Matrix",
           xlabel="Size of Matrix",
           ylabel="Time of Execution (ns)",
           xdata='Size',
           ydata='Time',
           hue=None,
           img_path=r'../images/turnaroundTime2ms.jpg')

plot_graph(df_path=r'../csvfiles/waitingTime_1ms.csv',
           title="Turnaround Time (quanta = 1ms) vs. Size of Matrix",
           xlabel="Size of Matrix",
           ylabel="Time of Execution (ns)",
           xdata='Size',
           ydata='Time',
           hue=None,
           img_path=r'../images/waitingTime1ms.jpg')

plot_graph(df_path=r'../csvfiles/waitingTime_2ms.csv',
           title="Turnaround Time (quanta = 2ms) vs. Size of Matrix",
           xlabel="Size of Matrix",
           ylabel="Time of Execution (ns)",
           xdata='Size',
           ydata='Time',
           hue=None,
           img_path=r'../images/waitingTime2ms.jpg')
