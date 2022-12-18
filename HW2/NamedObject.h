#pragma once

template <typename T>
class NamedObject {
	template <typename T> 
	friend class MyVector4;

private:
	T m_object;
	std::string m_name;

public:
	NamedObject(const T& object, const std::string& name) {
		if (name.empty()) {
			throw std::runtime_error("Named object must be fully initialized");
		}

		m_object = object;
		m_name = name;
	}

	void setObject(const T& other_object) {
		/*
			Detach of could be triggered here if NameObject had const pointer to its CowVector
			or through some Observer pattern. But I decided to not overcomplicate solution.
		*/
		m_object = other_object;
	}

	T& operator= (const T& other)
	{
		if (m_object == &other) // self-assignment guard
			return *this;

		/*
			Detach of could be triggered here if NameObject had const pointer to its CowVector
			or through some Observer pattern. But I decided to not overcomplicate solution.
		*/
		m_object = other;

		return *m_object;
	}

	void setName(const std::string& other_name) {
		/*
			Detach of could be triggered here if NameObject had const pointer to its CowVector
			or through some Observer pattern. But I decided to not overcomplicate solution.
		*/
		m_name = other_name;
	}

	const T& getObject() const { return m_object; }

	const std::string& getName() const { return m_name; }
};