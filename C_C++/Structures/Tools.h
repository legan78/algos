#ifndef TOOLS_H
#define TOOLS_H

template<size_t d = 1, typename type=float>
class Vector {
public:

	Vector() {
		for (size_t i = 0; i < d; i++)
			data[i] = 0;
	}

	Vector(const Vector& v) {
		for (size_t i = 0; i < d; i++)
			data[i] = v.data[i];
	}

	Vector<d, type>& operator=(const Vector<d, type>& v) {
		for (size_t i = 0; i < d; i++)
			data[i] = v.data[i];

		return *this;
	}

	type& operator[](size_t i) {
		return data[i];
	}

	const type& operator[](size_t i) const {
		return data[i];
	}

	void getBoundingBoxLimits(Vector<d, type>& mins, Vector<d, type>& maxs) const {
		for(size_t i=0; i< d; i++) { // It's a point!!
			mins[i] = maxs[i] = data[i];
		}
	}

protected:
	type data[d];
};




#endif
