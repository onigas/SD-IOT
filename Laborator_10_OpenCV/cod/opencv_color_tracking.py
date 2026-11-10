import cv2
import numpy as np


def main():
    cam_index = 0
    cap = cv2.VideoCapture(cam_index)

    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

    mode = "all"  # none, red, green, blue, all

    while True:
        ok, frame = cap.read()
        if not ok:
            print("Nu pot citi frame de la camera.")
            break

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # ---- intervale HSV stricte ----
        # rosu: saturat si destul de luminos
        lower_red1 = np.array([0,   150, 120])
        upper_red1 = np.array([10,  255, 255])
        lower_red2 = np.array([170, 150, 120])
        upper_red2 = np.array([180, 255, 255])

        # verde
        lower_green = np.array([40,  70,  70])
        upper_green = np.array([80, 255, 255])

        # albastru
        lower_blue = np.array([100,  70,  70])
        upper_blue = np.array([130, 255, 255])

        mask_red = np.zeros(hsv.shape[:2], dtype=np.uint8)
        mask_green = np.zeros_like(mask_red)
        mask_blue = np.zeros_like(mask_red)

        if mode in ("red", "all"):
            mask_red1 = cv2.inRange(hsv, lower_red1, upper_red1)
            mask_red2 = cv2.inRange(hsv, lower_red2, upper_red2)
            mask_red = cv2.bitwise_or(mask_red1, mask_red2)

        if mode in ("green", "all"):
            mask_green = cv2.inRange(hsv, lower_green, upper_green)

        if mode in ("blue", "all"):
            mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)

        kernel = np.ones((5, 5), np.uint8)

        def clean_mask(mask):
            m = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
            m = cv2.morphologyEx(m, cv2.MORPH_CLOSE, kernel)
            return m

        mask_red_clean = clean_mask(mask_red)
        mask_green_clean = clean_mask(mask_green)
        mask_blue_clean = clean_mask(mask_blue)

        contours_red, _ = cv2.findContours(
            mask_red_clean, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )
        contours_green, _ = cv2.findContours(
            mask_green_clean, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )
        contours_blue, _ = cv2.findContours(
            mask_blue_clean, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )

        h, w = frame.shape[:2]
        frame_area = w * h
        min_area = 0.02 * frame_area  # doar obiecte mari

        for cnt in contours_red:
            area = cv2.contourArea(cnt)
            if area < min_area:
                continue
            x, y, cw, ch = cv2.boundingRect(cnt)
            cv2.rectangle(frame, (x, y), (x + cw, y + ch), (0, 0, 255), 2)
            cv2.putText(
                frame,
                "RED",
                (x, y - 5),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (0, 0, 255),
                2,
            )

        for cnt in contours_green:
            area = cv2.contourArea(cnt)
            if area < min_area:
                continue
            x, y, cw, ch = cv2.boundingRect(cnt)
            cv2.rectangle(frame, (x, y), (x + cw, y + ch), (0, 255, 0), 2)
            cv2.putText(
                frame,
                "GREEN",
                (x, y - 5),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (0, 255, 0),
                2,
            )

        for cnt in contours_blue:
            area = cv2.contourArea(cnt)
            if area < min_area:
                continue
            x, y, cw, ch = cv2.boundingRect(cnt)
            cv2.rectangle(frame, (x, y), (x + cw, y + ch), (255, 0, 0), 2)
            cv2.putText(
                frame,
                "BLUE",
                (x, y - 5),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (255, 0, 0),
                2,
            )

        cv2.putText(
            frame,
            f"mode: {mode}   keys: r=red, g=green, b=blue, a=all, 0=none, q=quit",
            (10, h - 10),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.5,
            (255, 255, 255),
            1,
        )

        cv2.imshow("Color tracking (HSV)", frame)

        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):
            break
        elif key == ord("r"):
            mode = "red"
        elif key == ord("g"):
            mode = "green"
        elif key == ord("b"):
            mode = "blue"
        elif key == ord("a"):
            mode = "all"
        elif key == ord("0"):
            mode = "none"

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
