class Rectangle
{
public:
	Rectangle();

	Rectangle(int width, int height);

	int getArea() const;

	void set(int width, int height);

	static void standAloneFunction();

private:
	int mWidth;
	int mHeight;
};
