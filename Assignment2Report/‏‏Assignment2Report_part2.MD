

# ASSIGNMENT #2B


## Task 1
Ability to add point lights, ambient, diffuse and specular for both light and model are shown in the interface picture

![task1 light imgui interface](https://user-images.githubusercontent.com/92395711/147889560-1ffbe207-5f07-418f-99ad-936243f76de9.png)

Ambient lighting is like the light comes from th emodel himself,

Ahite light has all the components of the light and shows all the colors of the material.

Here is  picture of model with red ambient color illuminated by white light so the color we see is red.


![task 1ambient white light red material](https://user-images.githubusercontent.com/92395711/147889612-149bb80c-542f-446a-aa9d-c08098c01d2a.png)

Now the model is red and blue color with white light so we see the blend of red and blue

![task 1ambient white light red blue material](https://user-images.githubusercontent.com/92395711/147889628-2d415b2f-a311-4523-abeb-2053ea11d2cc.png)


Now the color of the light is red and blue but the moidel is blue and green so we see only blue

![task 1ambient green blue light red blue material](https://user-images.githubusercontent.com/92395711/147889671-8551adf2-bf32-4029-bc21-d13103a0314d.png)

Now light is green and the model is blue and red so we see no color (black)

![green light red blue material](https://user-images.githubusercontent.com/92395711/147889684-4959150b-2a05-4b7e-bece-1980fb4723bd.png)

## Task 2

Flat shading with diffuse component

Diffuse light from bottom

![task 2 light from bottom](https://user-images.githubusercontent.com/92395711/147889752-6f4d5301-3439-4880-aca1-d33b31d72b50.png)

 Two light from top corners

![task2 light from below](https://user-images.githubusercontent.com/92395711/147889711-e84bd8a9-ee81-42af-9e53-61c1a9e0b6d8.png)

Bunny

![task 2 bunny flat](https://user-images.githubusercontent.com/92395711/147889766-ac016e10-b066-4460-a67f-9dc572f3e392.png)


## Task 3 (pairs only)


## Task 4

I used the glm::reflect function and the parameters are the light direction and the face normal

In the picture below the light comes from the top left corner and the blue end of the short line is the direction of the reflection

![task 3 reflection from top right corner](https://user-images.githubusercontent.com/92395711/147889893-3c1666a2-a6e6-478b-b539-eeee4f967f6c.png)

## Task 5

Unfortunatly I could not get this to work, I tried to change angles and normals but I was Unlucky.
This was my calculation

![specular light calculation](https://user-images.githubusercontent.com/92395711/147889872-43e33e07-bbfb-4768-8a62-8364e6d947ff.png)

I go some nice results even without the specular lighting: 


![task 4 from lighting](https://user-images.githubusercontent.com/92395711/147889928-a29252df-3dd7-4320-9bf6-b8049d286c4b.png)

## Task 6 (pairs only)

## Task 7

![task 5 bunny front phong](https://user-images.githubusercontent.com/92395711/147889972-63add798-c6ba-47a4-845e-ad9dde5f36b8.png)

![task 5 bunny front flat](https://user-images.githubusercontent.com/92395711/147889973-5083e3cb-0aef-4366-bf32-68f484a0879f.png)

![task 5 chain flat](https://user-images.githubusercontent.com/92395711/147889974-30ca8c2f-41a5-4ff7-9e5f-777c17567481.png)

![task 5 pawn flat](https://user-images.githubusercontent.com/92395711/147889975-7f83878f-b03f-45f5-9c75-6c6373052cde.png)

![task 5 pawn phong](https://user-images.githubusercontent.com/92395711/147889976-fda73823-05e4-4ad6-a8ef-0fa29e906ff6.png)

![task 5 banana phong](https://user-images.githubusercontent.com/92395711/147889977-300b390a-5f74-4256-bc23-8523318fd0d0.png)

## Task 8
Could not complete this task as well, due to shortage in time

I did try to implement the fog this way:

![fog attempt](https://user-images.githubusercontent.com/92395711/147890037-56c4e9a8-48e0-4bcd-b748-c7fe78699fab.png)

I was not sure about the fog density value (b) and the result was not good enough for me to show you.










