import glnext
import imageio
import mymodule

windows = mymodule.init()
instance = glnext.instance(surface='VK_KHR_win32_surface')

image = instance.image((640, 360), mode='output')
buffer = instance.staging_buffer([[image]])

for wnd in windows:
    instance.surface(wnd, image)

reader = imageio.get_reader('assets/bunny.mp4')

for im in reader:
    buffer.write(glnext.rgba(im, 'rgb'))
    instance.run()
