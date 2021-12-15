

# ASSIGNMENT #2A
###### I submit the assignment 1 day after the deadline with the approval of Roi.

## Task 1
The way to implement it was very similar to the implementation of the big bounding box, I found the extreme points of every triangle and connected them using drawline. I implemented a way to toggle the feature on and off with imgui.

## Task 2
I chose to use scanline method.
I used the rectangle dimensions to know the size of boolean array that I needed for each triangle. The way I marked the triangle coordinates was following the path of the drawline algorithm and everytime I needed to light pixel, I just changed the value in the boolean array to TRUE. Then I ran with a loop on the lines and filled the coordinates between the values of the lines.

## Task 3
Using the boolean array, I ran again with a loop and everytime there was a TRUE value in boolean array[i][j] all I needed to do is to PutPixel( i, j, color).
(Maybe there was another way not to fill the boolean array with TRUE values and only mark the triangle coordinates. I was not sure but the implementation is basically the same, instead of fill the array with TRUE I could just put pixel there.)

## Task 4
I implemented z buffer according to the algorithm we leaned in class. The z buffer should light a pixel only if the z coordinate is closer then what already is lighted.
In order to find out the z coordinate of the a certain point I used Linear interpolation - Barycentric's method. Knowing the x,y coordinates and having the vector is enough because of the simple shape of triangle we can find the z computing the ratio of the coordinate compared to the vertices using areas. I used to formula we learned in class.
For colors with z buffer, instead of filling all triangle everytime I computed z coordinate and asked if it is closer to us then what we already are seeing. If it is put pixel and update z buffer.
The same was for gray scale image. instead of coloring it with the random color of the triangle I used camera.zFar to determine the position of the point in relation to zFar, the further the point is, the darker we will see it on the screen.

## photos

#### bounding rectangles:
![bounding_rectangles](https://user-images.githubusercontent.com/92395711/146228923-0fdf4fa1-1ed9-4ecd-b00f-4fc4f9e8a1c8.png)

#### Teapot without buffer:

![teapot_no_color_zbuffer](https://user-images.githubusercontent.com/92395711/146229485-9d82fc03-40bb-4480-aab2-3672f5554961.png)

#### Teapot with buffer:

![teapot_color_zbuffer](https://user-images.githubusercontent.com/92395711/146229579-e27d817f-6aba-4079-a186-10c4f711bf3e.png)


#### Teapot gray scale:

![teapot_gray](https://user-images.githubusercontent.com/92395711/146229622-ed7708ed-0cd4-45a5-82a0-373dae8e295b.png)

#### Bunny without buffer:

![bunny_color_no_zbuffer](https://user-images.githubusercontent.com/92395711/146229741-3dc48bde-dd84-43ef-962c-ca66d1444c13.png)

#### Bunny with buffer:

![bunny_color_zbuffer](https://user-images.githubusercontent.com/92395711/146229847-d20da8f0-89c8-4ab9-bcb0-c8741350ca40.png)


#### Bunny gray scale:

![bunny_gray](https://user-images.githubusercontent.com/92395711/146229925-94418689-e722-4c6e-a971-b543da67c4de.png)

#### Demo without buffer:

![demo_no_color_zbuffer](https://user-images.githubusercontent.com/92395711/146230028-6ded4b71-a40a-4604-be13-ec95b52eca20.png)

#### Demo with buffer:

![demo_color_zbuffer](https://user-images.githubusercontent.com/92395711/146230044-224e5c45-53be-43c3-8fa6-fe6dbb656af1.png)

#### Demo gray scale:

![demo_gray](https://user-images.githubusercontent.com/92395711/146230053-f2cb916c-ea83-4c37-b078-b9ae1f038509.png)







