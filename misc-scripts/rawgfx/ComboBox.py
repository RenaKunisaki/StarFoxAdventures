from gi.repository import Gtk
from Column import Column, TextColumn


class ComboBox(Gtk.ComboBox):
    def __init__(self, items, selected=0, onChange=None):
        model = self._buildModel(items)
        super().__init__(model=model, id_column=0)

        col = TextColumn(1, type=str, name="text")
        self.pack_start(col.renderer, True)
        self.add_attribute(col.renderer, 'text', 1)
        if selected is not None: self.set_active(selected)

        self.onChange = onChange
        self.connect('changed', self._on_changed)


    def _buildModel(self, items):
        """Build a model from the items given.

        This also replaces `self.itemIDs` and should only be used from
        `_replaceModel` or `__init__`.
        """
        model = Gtk.ListStore(str, str)
        self.itemIDs = []
        if type(items) is dict:
            for k, v in items.items():
                model.append((str(len(self.itemIDs)), v))
                self.itemIDs.append(k)
        else:
            for i, v in enumerate(items):
                self.itemIDs.append(i)
                model.append((str(i), v))
        return model


    def _replaceModel(self, items):
        """Replace the model with a new one."""
        newModel = self._buildModel(items)
        self.set_model(newModel)


    def _on_changed(self, cbo):
        if self.onChange is not None:
            self.onChange()
        return True

    @property
    def selected(self):
        id = int(self.get_active_id())
        try: return self.itemIDs[id]
        except IndexError: return None

    @selected.setter
    def selected(self, id):
        if type(id) is int:
            self.set_active_id(str(self.itemIDs[id]))
        else:
            self.set_active_id(str(self.itemIDs.index(id)))
