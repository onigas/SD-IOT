import cv2
import time
import os


def get_timestamp():
    return time.strftime("%Y%m%d_%H%M%S")


def main():
    cam_index = 0
    cap = cv2.VideoCapture(cam_index)

    # optional: rezolutie
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

    if not cap.isOpened():
        print("Nu pot deschide camera.")
        return

    # citim un frame ca sa aflam dimensiunea pt VideoWriter
    ret, frame = cap.read()
    if not ret:
        print("Nu pot citi primul frame.")
        return

    h, w = frame.shape[:2]
    frame_size = (w, h)

    # codec video – XVID sau MJPG merg de obicei bine pe RPi
    fourcc = cv2.VideoWriter_fourcc(*"XVID")
    video_writer = None
    recording = False

    print("Taste: s = snapshot, v = start/stop video, q = quit")

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Frame invalid, ies.")
            break

        # AICI poți insera procesare: edges, faces etc.
        display = frame.copy()

        # daca inregistram, scriem frame-ul in fisier
        if recording and video_writer is not None:
            video_writer.write(frame)
            cv2.putText(
                display,
                "REC",
                (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX,
                1.0,
                (0, 0, 255),
                2,
            )

        cv2.putText(
            display,
            "s: snapshot  |  v: rec on/off  |  q: quit",
            (10, h - 10),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.5,
            (255, 255, 255),
            1,
        )

        cv2.imshow("Camera capture", display)

        key = cv2.waitKey(1) & 0xFF

        if key == ord("q"):
            break

        elif key == ord("s"):
            # salvare snapshot
            ts = get_timestamp()
            filename = f"snapshot_{ts}.jpg"
            cv2.imwrite(filename, frame)
            print(f"Snapshot salvat: {filename}")

        elif key == ord("v"):
            # toggle video recording
            if not recording:
                # start recording
                ts = get_timestamp()
                video_filename = f"video_{ts}.avi"
                video_writer = cv2.VideoWriter(
                    video_filename, fourcc, 20.0, frame_size
                )
                if not video_writer.isOpened():
                    print("Nu pot crea fisier video.")
                    video_writer = None
                    recording = False
                else:
                    recording = True
                    print(f"Inregistrare START: {video_filename}")
            else:
                # stop recording
                recording = False
                if video_writer is not None:
                    video_writer.release()
                    video_writer = None
                print("Inregistrare STOP")

    cap.release()
    if video_writer is not None:
        video_writer.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
