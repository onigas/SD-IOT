import cv2
import numpy as np


def main():
    cam_index = 0  # 0 if you have a single USB camera
    cap = cv2.VideoCapture(cam_index)

    # optional: set resolution
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

    mode = "none"  # none, canny, sobel, laplace

    while True:
        ok, frame = cap.read()
        if not ok:
            print("Cannot read frame from camera.")
            break

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        if mode == "canny":
            blurred = cv2.GaussianBlur(gray, (5, 5), 0)
            edges = cv2.Canny(blurred, 50, 150)
            processed = edges

        elif mode == "sobel":
            gx = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=3)
            gy = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=3)
            mag = cv2.magnitude(gx, gy)
            processed = cv2.convertScaleAbs(mag)

        elif mode == "laplace":
            lap = cv2.Laplacian(gray, cv2.CV_64F, ksize=3)
            processed = cv2.convertScaleAbs(lap)

        else:
            processed = gray  # just grayscale

        proc_bgr = cv2.cvtColor(processed, cv2.COLOR_GRAY2BGR)
        combined = np.hstack((frame, proc_bgr))

        cv2.imshow("Camera (left) + processed (right)", combined)

        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):
            break
        elif key == ord("0"):
            mode = "none"
        elif key == ord("1"):
            mode = "canny"
        elif key == ord("2"):
            mode = "sobel"
        elif key == ord("3"):
            mode = "laplace"

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
