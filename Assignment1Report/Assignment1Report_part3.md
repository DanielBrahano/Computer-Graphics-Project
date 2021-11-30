

# ASSIGNMENT #1B
###### Mesh is loaded through File button and then choose the path to the model.

## Task 1
I removed the centering trick as requested, I implemented orthographic projection with adjustable view volume(can be changed with the gui sliders). Canonical view as default.
In the first picture we see the default view volume, In the second as I increase the view volume the mesh looks smaller. Near and Far have no effect.

![part1_view_volume](https://user-images.githubusercontent.com/92395711/144024141-98c88dbb-25af-4e8d-89eb-5a368fa50d65.png)


![part1_another_view_volume](https://user-images.githubusercontent.com/92395711/144024259-006c2065-469f-4db0-925e-aac7dd6133fb.png)


## Task 2
In order to change the view volume according to the window size, I had to go to RendererFrame function and get the information about the window size, from there I created a method that let the renderer know that the size has change and to display the mesh according to the new window size.


![part2_view_volume_auto_change1](https://user-images.githubusercontent.com/92395711/144024962-187c3152-7972-4ae3-9801-d7b4d1987f3f.png)


![part2_view_volume_auto_change2](https://user-images.githubusercontent.com/92395711/144024997-5960a79f-3a4a-4041-aef6-de32972dfcdf.png)

## Task 3
As it can be seen from the pictures, in the first one is the default, then translate in the world frame so the model axes move with the model, then translate and rotate in the model frame and it the model axes stay fix.


![part3_before_transform](https://user-images.githubusercontent.com/92395711/144025634-e40acb1f-4c27-4d41-9fe5-56da8a600749.png)


![part3_first_world_translate](https://user-images.githubusercontent.com/92395711/144025684-6ce067bb-369b-4d9e-a2e8-33707c395009.png)


![part3_then_local_translate rotate](https://user-images.githubusercontent.com/92395711/144025830-21ceb83a-ccbc-4420-8539-7f8e542facee.png)


## Task 4
T1 = zRotate

T2 = xTranslate

In the first picture I applied T1 on world frame then T2 on local frame.

in the second one I applied T1 on local frame and T2 on world frame.

Different positions of the model.


![part4_banana_world_xtranslate_local_zrotate](https://user-images.githubusercontent.com/92395711/144029450-eca384a8-ca9d-4a3e-85c0-db2691d02fc9.png)



![part4_banana_local_xtranslate_world_zrotate](https://user-images.githubusercontent.com/92395711/144029494-b886815d-ae08-4555-85e8-b952bb4a4fe1.png)

## Task 5

Here are pictures of bounding box,vertex normals and face normals before and after some transformationas.

![part5_box](https://user-images.githubusercontent.com/92395711/144038541-9b6fc341-77c4-4cb9-b9b5-def21f1f62b9.png)

![part5_box_after_transformation](https://user-images.githubusercontent.com/92395711/144038573-9dfffd93-f49f-4114-89a4-5c665abb3cde.png)

![part5_face_normals](https://user-images.githubusercontent.com/92395711/144038596-7f61d782-94db-4f3e-aaa5-483eb79af437.png)

![part5_face_normals_after transformations](https://user-images.githubusercontent.com/92395711/144038617-5ba73cff-e5e3-4288-865f-fb4c58725b58.png)

![part5_vertex_normals](https://user-images.githubusercontent.com/92395711/144038655-fd7996a6-bc6e-4d4f-8294-8c347d481a09.png)

![part5_vertex_normals_after_transformations](https://user-images.githubusercontent.com/92395711/144038700-0b09a5ae-bc84-43cb-8190-14af19e9ac4e.png)


## Task 6

Perspective vs orthographic

![part6_orthographic](https://user-images.githubusercontent.com/92395711/144038877-03805fde-6d5f-4f8f-a380-f9fd80c94e00.png)

![part6_peprspective](https://user-images.githubusercontent.com/92395711/144038898-dff27205-cd99-4805-89b9-49f96afde833.png)

## Task 7

Starting position:

![part7_start](https://user-images.githubusercontent.com/92395711/144039108-d476a3d9-80c3-4849-aa3e-3c50c955476a.png)

Zoom:

![part7_change_fovy](https://user-images.githubusercontent.com/92395711/144039155-501ef405-5733-429e-8a3c-2e06a49fd1dd.png)


Moving closer:

![part7_change_distance](https://user-images.githubusercontent.com/92395711/144039186-c01253d8-5d74-4006-ba11-02f3ffed2c56.png)


## Task 8

Start:

![part8_start](https://user-images.githubusercontent.com/92395711/144039512-812e53a0-27c1-4f46-af01-38176410a13a.png)


Camera rotate in world:


![part8_rotate_camera_ world_frame](https://user-images.githubusercontent.com/92395711/144039643-96c37812-83e1-4304-8e05-41da6d485031.png)



Cmera rotate around itself:

![part8_rotate_camera_ local_frame](https://user-images.githubusercontent.com/92395711/144039559-41e104d9-feda-46e0-87c2-e5872910bed1.png)



## Task 12

![part10_interface](https://user-images.githubusercontent.com/92395711/144039848-80044ee0-373e-48b5-b544-0b2eacab7657.png)

## Task 13
I implemented a way to transform the model with the keyboard, checkboxes for drawing bounding box, vertex normals, face normals, world and local axes for the model.