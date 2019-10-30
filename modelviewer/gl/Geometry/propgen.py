'''
Property Generation

These functions try to avoid some of the boilerplate of using
@property declarations within classes.

'''

from .constants import nearly_eq, nearly_zero, Epsilon

import collections


# still sort of boilerplate-y but getting better
#
# EJO abstract property creation by having the caller
#     supply data validation functions.  the caller
#     becomes more complicated again, but has more
#     control of property implementation details and
#     propgen becomes less complicated.

# EJO the ultimate goal is to make propgen go away


def FloatProperty(name, default=0.0, readonly=False, docs=None):
    '''
    :name: string - property name
    :default: float - property default value
    :readonly: boolean - if True, setter method is NOT generated

    Returns a property object that can be used to initialize a
    class instance variable as a property.
    '''

    private_name = '_' + name

    def getf(self):
        if not hasattr(self, private_name):
            setattr(self, private_name, default)
        return getattr(self, private_name)

    if readonly:
        setf = None
    else:
        def setf(self, newValue):
            def epsilon_set(v):
                # epsilon_set: creates a float from v unless that
                #              float is less than epsilon, which will
                #              be considered effectively zero.
                fv = float(v)
                return 0.0 if nearly_zero(fv) else fv

            try:
                setattr(self, private_name, epsilon_set(newValue))
                return
            except TypeError:
                pass

            if isinstance(newValue, collections.Mapping):
                try:
                    setattr(self, private_name, epsilon_set(newValue[name]))
                except KeyError:
                    pass
                return

            if isinstance(newValue, collections.Iterable):
                try:
                    setattr(self, private_name, epsilon_set(newValue[0]))
                    return
                except (IndexError, TypeError):
                    pass

            try:
                mapping = vars(newValue)
                setattr(self, private_name, epsilon_set(mapping[name]))
                return
            except (TypeError, KeyError):
                pass

            if newValue is None:
                setattr(self, private_name, epsilon_set(default))
                return

            raise ValueError(newValue)

    return property(getf, setf, None, docs)


def FloatMultiProperty(keys, default=0.0, docs=None,
                       readonly_keys=''):
    '''


    '''

    writable_keys = [k for k in keys if k not in readonly_keys]

    def getf(self):
        return list(self[key] for key in keys)

    def setf(self, newValues):

        if newValues is None:
            for key in writable_keys:
                setattr(self, key, default)
            return

        if isinstance(newValues, collections.Mapping):
            for key, value in newValues.items():
                if key in writable_keys:
                    setattr(self, key, value)
            return

        if isinstance(newValues, collections.Iterable):
            for key, value in zip(writable_keys, newValues):
                setattr(self, key, value)
            return

        try:
            mapping = vars(newValues)
            for key, value in mapping.items():
                if key in writable_keys:
                    setattr(self, key, value)
            return
        except TypeError:
            pass

        setattr(self, writable_keys[0], newValues)

    return property(getf, setf, None, docs)


def MultiObjectProperty(keys, default, all_keys):

    if not any(set(keys).intersection(all_keys)):
        raise KeyError('{!r} not subset of {!r}'.format(keys, all_keys))

    @property
    def prop(self):
        return list(self[key] for key in keys)

    @prop.setter
    def prop(self, newValues):

        if newValues is None:
            for key in keys:
                setattr(self, key, default)
            return

        if isinstance(newValues, collections.Mapping):
            for key, value in newValues.items():
                if key in keys:
                    setattr(self, key, value)
            return

        if isinstance(newValues, collections.Iterable):
            for key, value in zip(keys, newValues):
                setattr(self, key, value)
            return

        try:
            mapping = vars(newValues)
            for key, value in mapping.items():
                if key in keys:
                    setattr(self, key, value)
            return
        except TypeError:
            pass

        setattr(self, keys[0], newValues)

    return prop
