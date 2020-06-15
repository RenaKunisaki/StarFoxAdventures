"""A column in a list view."""
from gi.repository import Gtk


class Column(Gtk.TreeViewColumn):
    defaults = dict(
        type   = str, # data type
        xalign = 0.0, # X alignment (0=left, 1=right)
        yalign = 0.5, # Y alignment (0=top,  1=bottom)
        resizable = True,
        reorderable = True,
        show = True, # is column visible?
        func = None, # function to get col value from object
    )

    def __init__(self, number, **attrs):
        self.number = number

        # get the attributes and their defaults
        atr = self.defaults.copy()
        atr.update(attrs)
        if 'displayName' not in atr:
            atr['displayName'] = atr['name']

        if 'sort_column_id' not in atr:
            atr['sort_column_id'] = number # sort by self
        elif atr['sort_column_id'] is None:
            del atr['sort_column_id'] # don't allow sorting

        # we must construct the TreeViewColumn before attempting to
        # set any attributes, and it requires a name.
        # passing the other attributes here doesn't work, because
        # it wants to use gtk_tree_view_column_add_attribute(). (bug?)
        super().__init__(atr['displayName'], self.renderer,
            text=self.number)

        # now set the other attributes.
        for name, val in atr.items():
            try: self.set_property(name, val)
            except TypeError: setattr(self, name, val)


    @property
    def sort_column_id(self):
        """The column ID to sort on.

        This is a property of the column, because a column can sort
        based on the contents of another column. This is helpful if
        your column displays formatted data but needs to sort on the
        raw data (which you can store in another, hidden column),
        such as file sizes:

        Size  RawSize
        2K       2048
        1M    1048576

        1M shouldn't sort before 2K, so we can use RawSize (which
        would be hidden) as the sort column for Size to get correct
        sorting.
        """
        return self.get_sort_column_id()

    @sort_column_id.setter
    def sort_column_id(self, col):
        self.set_sort_column_id(col)


    @property
    def on_edit(self):
        """Function to call when this column is edited.

        If None (the default), editing is disabled.
        """
        return self._on_edit

    @on_edit.setter
    def on_edit(self, func):
        self._on_edit = func
        if func is None:
            self.renderer.set_property('editable', False)
        else:
            self.renderer.connect('edited', func)
            self.renderer.set_property('editable', True)


    @property
    def data_func(self):
        """Function that provides the data to display.

        If None, it will just display the raw data from the associated
        column of the tree model. You can use this hook to format the
        data (but also see `render_func` and `fmt`).
        """
        return self.get_cell_data_func(self.renderer)

    @data_func.setter
    def data_func(self, func):
        self.set_cell_data_func(self.renderer, func)


    @property
    def render_func(self):
        """Function that takes the cell's raw data and returns
        formatted text to display.

        This is usually nicer than using `data_func` since it takes
        care of retrieving the value and setting the text for you.

        Setting this actually sets a new `data_func`.
        """
        return self._render_func

    @render_func.setter
    def render_func(self, func):
        self._render_func = func
        def _render(col, cell, model, iter, udata):
            text = str(self.render(col, model.get_value(iter, self.number)))
            cell.set_property('text', text)
        self.data_func = _render


    @property
    def fmt(self):
        """Format string for displaying the cell's data.

        This is an alternative to `render_func` for simple formatting.
        The given format string is used (with the `%` operator) to
        format the cell's raw value and generate the displayed text.
        eg if a column's type is `int`, you can set this to `%X` to
        display it in hex.

        Note that the types must match; eg using `%X` with a column of
        type `str` won't work very well.

        Setting this actually sets a new `data_func`.
        """
        return self._fmt

    @fmt.setter
    def fmt(self, fmt):
        self._fmt = fmt
        if callable(fmt):
            def _render(col, cell, model, iter, udata):
                text = fmt(model.get_value(iter, self.number))
                cell.set_property('text', text)
        else:
            def _render(col, cell, model, iter, udata):
                text = fmt % model.get_value(iter, self.number)
                cell.set_property('text', text)
        self.data_func = _render



class TextColumn(Column):
    """A column that displays plain text.

    This text can be edited, if `on_edit` is set.
    """
    def __init__(self, number, **attrs):
        self.renderer = Gtk.CellRendererText(
            xalign = attrs.get('xalign', 0.0),
            yalign = attrs.get('yalign', 0.5),
        )
        super().__init__(number, **attrs)
