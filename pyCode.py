""" This program accepts input 2 python lists and return two new python list.
    First list consist of addition items from input lists,
    Second list consist of subtract items from input lists.
"""

def func(arr1:list, arr2:list):
    assert(len(arr1) == len(arr2));
    summary = [arr1[i] + arr2[i] for i in range(len(arr1))]
    subtract = [arr1[i] - arr2[i] for i in range(len(arr1))]
    return summary, subtract
