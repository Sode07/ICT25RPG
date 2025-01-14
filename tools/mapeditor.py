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

        self.current_depth_label = tk.Label(root, text="Current Depth: 0")
        self.current_depth_label.pack()

        self.save_button = tk.Button(root, text="Tallenna", command=self.save_map)
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

        self.depth_label = tk.Label(root, text="Depth:")
        self.depth_label.pack()
        self.depth_entry = tk.Entry(root)
        self.depth_entry.insert(0, "2")
        self.depth_entry.pack()

        self.name_label = tk.Label(root, text="Nimi:")
        self.name_label.pack()
        self.name_entry = tk.Entry(root)
        self.name_entry.insert(0, "default")
        self.name_entry.pack()

        self.update_button = tk.Button(root, text="Päivitä", command=self.update_grid)
        self.update_button.pack()

        self.depth_up_button = tk.Button(root, text="Ylös", command=self.depth_up)
        self.depth_up_button.pack()

        self.depth_down_button = tk.Button(root, text="Alas", command=self.depth_down)
        self.depth_down_button.pack()

        self.number_label = tk.Label(root, text="Piirrä:")
        self.number_label.pack()
        self.number_entry = tk.Entry(root)
        self.number_entry.insert(0, "1")
        self.number_entry.pack()

        self.canvas.bind("<Button-1>", self.on_left_click)
        self.canvas.bind("<Button-3>", self.on_right_click)

        self.layers = []
        self.current_depth = 0

    def load_map(self):
        file_path = filedialog.askopenfilename(defaultextension=".juusto", filetypes=[("Kartta filut", "*.juusto")])
        if not file_path:
            return

        with open(file_path, "r") as file:
            lines = file.readlines()

        header = lines[0].strip().split(",")
        width = int(header[1][1:])
        height = int(header[2][1:])
        depth = int(header[3][1:])
        name = header[4][1:-1]

        self.width_entry.delete(0, tk.END)
        self.width_entry.insert(0, str(width))
        self.height_entry.delete(0, tk.END)
        self.height_entry.insert(0, str(height))
        self.depth_entry.delete(0, tk.END)
        self.depth_entry.insert(0, str(depth))
        self.name_entry.delete(0, tk.END)
        self.name_entry.insert(0, name)

        self.layers = []
        for d in range(depth):
            layer_data = []
            for i in range(height):
                row_data = list(map(int, lines[1 + i + d * height].strip().split(",")[1:-1]))
                layer_data.append(row_data)
            self.layers.append(layer_data)

        self.current_depth = 0
        self.update_grid()

    def update_grid(self):
        self.canvas.delete("all")
        try:
            width = int(self.width_entry.get())
            height = int(self.height_entry.get())
            depth = int(self.depth_entry.get())
        except ValueError:
            messagebox.showerror("Error", "Width, height, or depth is invalid.")
            return

        cell_size = 50
        self.cells = []
        for i in range(height):
            row = []
            for j in range(width):
                rect = self.canvas.create_rectangle(j * cell_size, i * cell_size, (j + 1) * cell_size, (i + 1) * cell_size, outline="black", fill="white")
                row.append(rect)
            self.cells.append(row)

        self.canvas.config(scrollregion=self.canvas.bbox("all"))

        if len(self.layers) == 0:
            self.layers = [[[0] * width for _ in range(height)] for _ in range(depth)]

        self.display_layer(self.current_depth)

    def display_layer(self, depth):
        if 0 <= depth < len(self.layers):
            self.current_depth = depth
            self.current_depth_label.config(text=f"Tän hetkine syvyys: {self.current_depth}")
            for i in range(len(self.cells)):
                for j in range(len(self.cells[0])):
                    value = self.layers[depth][i][j]
                    color = "white" if value == 0 else "green" if value == 1 else "red"
                    self.canvas.itemconfig(self.cells[i][j], fill=color)

    def on_left_click(self, event):
        self.change_cell_color(event, "green")

    def on_right_click(self, event):
        self.change_cell_color(event, "red")

    def change_cell_color(self, event, color):
        cell_size = 50
        x = event.x // cell_size
        y = event.y // cell_size
        if 0 <= x < len(self.cells[0]) and 0 <= y < len(self.cells):
            self.canvas.itemconfig(self.cells[y][x], fill=color)
            value = 1 if color == "green" else 2
            self.layers[self.current_depth][y][x] = value

    def save_map(self):
        file_path = filedialog.asksaveasfilename(defaultextension=".juusto", filetypes=[("Kartta filut", "*.juusto")])
        if not file_path:
            return

        try:
            width = int(self.width_entry.get())
            height = int(self.height_entry.get())
            depth = int(self.depth_entry.get())
            name = self.name_entry.get()
        except ValueError:
            messagebox.showerror("Error", "Width, height, or depth is invalid.")
            return

        with open(file_path, "w") as file:
            file.write(f",w{width},h{height},d{depth},\"{name}\",\n")
            for layer in self.layers:
                for row in layer:
                    file.write(",")
                    file.write(",".join(map(str, row)))
                    file.write(",\n")

        messagebox.showinfo("Onnistui", "Kartta tallennettu!")

    def depth_up(self):
        if self.current_depth > 0:
            self.current_depth -= 1
            self.display_layer(self.current_depth)

    def depth_down(self):
        if self.current_depth < len(self.layers) - 1:
            self.current_depth += 1
            self.display_layer(self.current_depth)

if __name__ == "__main__":
    root = tk.Tk()
    editor = MapEditor(root)
    root.mainloop()
