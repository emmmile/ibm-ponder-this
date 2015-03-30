#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random


def single():
	return sorted(random.sample(range(1,32), 6))

def intersection(a,b):
	i = [val for val in b if val in a]
	return len(i)

def next(faces):
	while True:
		face = single()

		notok = [x for x in faces if intersection(face,x) != 1]
		if len(notok) == 0:
			return face

def compute():
	faces = list()
	while len(faces) < 25:
		ok = next(faces)
		print(ok)
		faces.append(ok)

	return faces

compute()