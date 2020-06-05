#ifndef MY_PROJECT_PATH_RECTANGLE_HXX
#define MY_PROJECT_PATH_RECTANGLE_HXX

class Rectangle
{
public:
	Rectangle();

	Rectangle(int width, int height);

	Rectangle(const Rectangle& object);

	int getArea() const;

	Rectangle& operator=(const Rectangle& object);

	void set(int width, int height);

	static void standAloneFunction();

private:
	int mWidth;
	int mHeight;

protected:
	int mArea;
};

#endif
