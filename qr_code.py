import cv2
import numpy as np
import time


cam_url = "http://192.168.4.3:81/stream"
cap = cv2.VideoCapture(cam_url)
qr = cv2.QRCodeDetector()

if not cap.isOpened():
    print("Could not open camera stream")
    raise SystemExit

def order_corners(pts: np.ndarray) -> np.ndarray:
    """
    Order points as: top-left, top-right, bottom-left, bottom-right.
    """
    pts = pts.reshape(-1, 2).astype(float)
    rect = np.zeros((4, 2), dtype=float)

    s = pts.sum(axis=1)
    rect[0] = pts[np.argmin(s)]  # top-left  (min x+y)
    rect[3] = pts[np.argmax(s)]  # bottom-right (max x+y)

    d = np.diff(pts, axis=1).ravel()  # (y - x)
    rect[1] = pts[np.argmin(d)]  # top-right (min diff)
    rect[2] = pts[np.argmax(d)]  # bottom-left (max diff)
    return rect

while True:
    ok, frame = cap.read()
    if not ok:
        print("Failed to read frame")
        break

    data, points, _ = qr.detectAndDecode(frame)

    if data and points is not None:
        # Normalise shape to (4, 2)
        pts = np.squeeze(points)            # handles (1,4,2) -> (4,2)
        if pts.shape != (4, 2):
            # Safety check in case the API returns an unexpected shape
            pts = pts.reshape(-1, 2)[:4]

        ordered = order_corners(pts)
        corners_int = np.rint(ordered).astype(int)

        # Centre as average of corners
        centre = np.mean(ordered, axis=0)
        centre_int = np.rint(centre).astype(int)

        # Prepare printout in the requested format
        tl, tr, bl, br = corners_int  # top-left, top-right, bottom-left, bottom-right

        print(f"QR detected: {data}")
        print(f"qr_centre: [{centre_int[0]}, {centre_int[1]}]")
        print(
            "qr_corner = "
            f"[[[{tl[0]},{tl[1]}], [{tr[0]},{tr[1]}]], "
            f"[[{bl[0]},{bl[1]}], [{br[0]},{br[1]}]]]"
        )

        # Draw the outline and centre for debugging
        cv2.polylines(frame, [corners_int.reshape(-1, 1, 2)], isClosed=True, color=(0, 255, 0), thickness=2)
        cv2.circle(frame, (centre_int[0], centre_int[1]), 4, (0, 0, 255), -1)

    cv2.imshow("ESP32-CAM", frame)
    if cv2.waitKey(1) & 0xFF == 27:   # ESC to quit
        break


cap.release()
cv2.destroyAllWindows()
