import time
from qr_code import scan_qr


def sequence(forward, left, right, left_turn, right_turn):
    # initialization
    frame_size = (640, 480)  # horizontal, vertical
    frame_center = (frame_size[0] / 2, frame_size[1] / 2)
    offset = 20  # we could keep the edge 20 pixels away from the edge, remember to test this value out
    tolerance = 10
    sleep_time = 0.01

    # search qr
    qr_output = scan_qr()
    while not qr_output:  # continuous scanning for qr code
        left_turn()
        qr_output = scan_qr()
        time.sleep(sleep_time)

    qr_center, qr_corner = scan_qr()
    qr_left_height = qr_corner[0][0][1] - qr_corner[1][0][1]
    qr_right_height = qr_corner[0][1][1] - qr_corner[1][1][1]

    # right turn maneuver
    if qr_left_height <= qr_right_height:
        # align qr edge and frame edge
        # stop when qr code right edge reaches frame right edge
        while qr_corner[0][1][0] < frame_size[0] - offset:
            left_turn()
            qr_center, qr_corner = scan_qr()
            time.sleep(sleep_time)
        # path finding
        # stop when qr code left edge reach left of frame center
        while qr_corner[0][0][0] > frame_center[0]:
            forward()
            qr_center, qr_corner = scan_qr()
            if qr_corner[0][1][0] >= frame_size[0] - offset:
                right_turn()
            time.sleep(sleep_time)
        qr_left_height = qr_corner[0][0][1] - qr_corner[1][0][1]
        qr_right_height = qr_corner[0][1][1] - qr_corner[1][1][1]
        # final adjustments with angle
        # stop when left/right height difference within tolerance or left edge touchs
        while abs(qr_left_height - qr_right_height) <= tolerance or qr_corner[0][0][0] <= offset:
            right_turn()
            qr_center, qr_corner = scan_qr()
            qr_left_height = qr_corner[0][0][1] - qr_corner[1][0][1]
            qr_right_height = qr_corner[0][1][1] - qr_corner[1][1][1]
            time.sleep(sleep_time)

    # left turn maneuver
    elif qr_left_height > qr_right_height:
        # align qr edge and frame edge
        while qr_corner[0][0][1] > offset:  # stop when qr left edge reaches frame left edge
            right_turn()
            qr_center, qr_corner = scan_qr()
            time.sleep(sleep_time)
        # path finding
        # stop when qr code right edge reach right of frame center
        while qr_corner[0][1][0] < frame_center[0]:
            forward()
            qr_center, qr_corner = scan_qr()
            if qr_corner[0][0][1] <= offset:
                left_turn()
            time.sleep(sleep_time)
        qr_left_height = qr_corner[0][0][1] - qr_corner[1][0][1]
        qr_right_height = qr_corner[0][1][1] - qr_corner[1][1][1]
        # final adjustments with angle
        # stop when left/right height difference within tolerance or right edge touches
        while abs(qr_left_height - qr_right_height) <= tolerance or qr_corner[0][1][0] >= frame_size[0] - offset:
            right_turn()
            qr_center, qr_corner = scan_qr()
            qr_left_height = qr_corner[0][0][1] - qr_corner[1][0][1]
            qr_right_height = qr_corner[0][1][1] - qr_corner[1][1][1]
            time.sleep(sleep_time)

    # final adjustment to center the dock
    while abs(qr_center - frame_center) <= tolerance:
        if qr_center < frame_center:
            right()
        elif qr_center > frame_center:
            left()
        qr_center, qr_corner = scan_qr()
        time.sleep(sleep_time)