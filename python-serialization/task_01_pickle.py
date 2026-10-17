#!/usr/bin/env python3
"""Serialization and deserialization of custom objects."""

import pickle


class CustomObject:
    """A custom class for storing personal information."""

    def __init__(self, name, age, is_student):
        self.name = name
        self.age = age
        self.is_student = is_student

    def display(self):
        """Display the object's attributes."""
        print(f"Name: {self.name}")
        print(f"Age: {self.age}")
        print(f"Is Student: {self.is_student}")

    def serialize(self, filename):
        """Serialize the object to a binary file."""
        try:
            with open(filename, "wb") as file:
                pickle.dump(self, file)
        except (OSError, pickle.PickleError, TypeError,
                AttributeError):
            return None

    @classmethod
    def deserialize(cls, filename):
        """Deserialize an object from a binary file."""
        try:
            with open(filename, "rb") as file:
                obj = pickle.load(file)
            if isinstance(obj, cls):
                return obj
            return None
        except (OSError, pickle.UnpicklingError, EOFError,
                AttributeError, ImportError, IndexError,
                ValueError):
            return None
