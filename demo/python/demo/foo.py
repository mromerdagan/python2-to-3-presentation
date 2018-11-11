
def print_hello(name):
	print("Hello", end=' ')
	print(name)

def filter_even(l):
	ret = [x for x in l if x % 2 == 0]
	return ret

def floor_division(x, y):
	ret = x // y
	return ret
