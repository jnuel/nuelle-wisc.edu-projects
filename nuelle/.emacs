;; CSL UNIX .emacs
;;
;; .emacs file
;; $Id: .emacs,v 1.18 2004/01/15 19:54:44 john Exp $
;;
;; This file contains customization information for the text editor 
;; emacs. The language that is used is called elisp, which is a emacs
;; version of lisp. 


;; ###########################################################################
;; The following two lines fix a problem that some people have. The original
;; emacs mapped C-h to be the help screen. Unfortunately, this is the same 
;; as backspace. So, attempting to press backspace would bring up a help
;; screen. The new key-binding for help is C-x h.
(global-set-key "\^xh" 'help-for-help)
(global-set-key "\^h"  'backward-delete-char-untabify)

;; 
;; require-final-newline ensures that every line of a text file ends with a
;; "newline" characted. Many unix utilities need a final newline in order to
;; read the last line of the file. You probably don't want to change this.
(setq require-final-newline t)

;; Uncomment the following to see the line number, column number and time
;(line-number-mode t)
;(column-number-mode t)
;(display-time)

;; If you would like smooth scrolling, uncomment this line
;(setq scroll-step 1)

;; For a much better buffer list:
(global-set-key "\C-x\C-b" 'electric-buffer-list)

;; ###########################################################################
;; The following is an example of how to customize the c-mode. There are
;; also different modes for asm, c++, lisp, perl, tcl, tex, etc...

;; you can have GNU, K&R, BSD, C++, Whitesmith
;(setq default-c-style 'K&R)

;; another useful option
;(setq c-auto-newline t)


;;; Re-enable from "novice"

(put 'eval-expression 'disabled nil)
(put 'downcase-region 'disabled t)      ;; It's very annoying to do this
(put 'upcase-region 'disabled t)        ;; by accident!

;; Options Menu Settings
;; =====================
; no longer needed as of xemacs 21.4
;(cond
; ((and (string-match "XEmacs" emacs-version)
;       (boundp 'emacs-major-version)
;       (or (and
;            (= emacs-major-version 19)
;            (>= emacs-minor-version 14))
;           (= emacs-major-version 20))
;       (fboundp 'load-options-file))
;  (load-options-file "~/.xemacs-options")))
;; ============================
;; End of Options Menu Settings

; load variable config for xemacs (needs the customize package)
;(cond
; ((and (string-match "XEmacs" emacs-version)
;       (boundp 'emacs-major-version)
;       (or (and
;            (= emacs-major-version 19)
;            (>= emacs-minor-version 14))
;           (= emacs-major-version 20))
;       (fboundp 'load-options-file))
;  (load-options-file "~/.xemacs")))

;; ###########################################################################
;; add some colors to your emacs (not xemacs) session
(cond ((and (not (string-match "XEmacs" emacs-version)) window-system)

        (global-font-lock-mode t)
        (setq font-lock-maximum-decoration t)

; some other font-lock stuff you can set

;        (setq font-lock-maximum-decoration 2)
;        (font-lock-make-faces t)
;        (setq font-lock-face-attributes
;              '((font-lock-comment-face "Firebrick")
;                (font-lock-string-face "RosyBrown")
;                (font-lock-keyword-face "Purple")
;                (font-lock-function-name-face "Blue")
;                (font-lock-variable-name-face "DarkGoldenrod")
;                (font-lock-type-face "DarkOliveGreen")
;                (font-lock-reference-face "CadetBlue")))
))

