import tkinter as tk
from tkinter import filedialog, messagebox, Scrollbar, HORIZONTAL, VERTICAL

class MapEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("Map Editor")

        self.canvas_frame = tk.Frame(root)
        self.canvas_frame.pack(fill="both", expand=True)

        self.canvas = tk.Canvas(self.canvas_frame, bg="white")
        self.canvas.pack(side="left", fill="both", expand=True)

        self.scroll_x = Scrollbar(self.canvas_frame, orient="horizontal", command=self.canvas.xview)
        self.scroll_x.pack(side="bottom", fill="x")

        self.scroll_y = Scrollbar(self.canvas_frame, orient="vertical", command=self.canvas.yview)
        self.scroll_y.pack(side="right", fill="y")

        self.canvas.configure(xscrollcommand=self.scroll_x.set, yscrollcommand=self.scroll_y.set)

        self.current_depth_label = tk.Label(root, text="Tän hetkine syvyys: 0")
        self.current_depth_label.pack()

        self.save_button = tk.Button(root, text="Tallenna")
        self.save_button.pack()

        self.load_button = tk.Button(root, text="Lataa", command=self.load_map)
        self.load_button.pack()

        self.width_label = tk.Label(root, text="Leveys:")
        self.width_label.pack()
        self.width_entry = tk.Entry(root)
        self.width_entry.insert(0, "9")
        self.width_entry.pack()

        self.height_label = tk.Label(root, text="Korkeus:")
        self.height_label.pack()
        self.height_entry = tk.Entry(root)
        self.height_entry.insert(0, "7")
        self.height_entry.pack()

        self.depth_label = tk.Label(root, text="Syvyys:")
        self.depth_label.pack()
        self.depth_entry = tk.Entry(root)
        self.depth_entry.insert(0, "2")
        self.depth_entry.pack()

        self.name_label = tk.Label(root, text="Nimi:")
        self.name_label.pack()
        self.name_entry = tk.Entry(root)
        self.name_entry.insert(0, "oletus")
        self.name_entry.pack()

        self.update_button = tk.Button(root, text="Päivitys")
        self.update_button.pack()

        self.depth_up_button = tk.Button(root, text="Ylös")
        self.depth_up_button.pack()

        self.depth_down_button = tk.Button(root, text="Alas")
        self.depth_down_button.pack()

        self.number_label = tk.Label(root, text="Mitäs piirretää:")
        self.number_label.pack()
        self.number_entry = tk.Entry(root)
        self.number_entry.insert(0, "1")
        self.number_entry.pack()

        self.zoom_in_button = tk.Button(root, text="Sisään")
        self.zoom_in_button.pack()

        self.zoom_out_button = tk.Button(root, text="Nääsis")
        self.zoom_out_button.pack()
    def load_map():
        print("test")

if __name__ == "__main__":
    root = tk.Tk()
    editor = MapEditor(root)
    root.mainloop()
