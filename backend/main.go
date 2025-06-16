package main

import (
	"encoding/json"
	"log"
	"net/http"
)

const esp32URL = "ESP32_IP-ADDRESS"

type ColorRequest struct {
	Color string `json:"color"`
}

type PatternRequest struct {
	Pattern string `json:"pattern"`
}

func setColorHandler(w http.ResponseWriter, r *http.Request) {
	var req ColorRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		http.Error(w, "Bad Request", http.StatusBadRequest)
		return
	}
	resp, err := http.Post(esp32URL+"/color", "application/json", r.Body)
	if err != nil {
		http.Error(w, "Failed to contact ESP32", http.StatusInternalServerError)
		return
	}
	defer resp.Body.Close()
	w.WriteHeader(http.StatusOK)
}

func main() {
	http.Handle("/", http.FileServer(http.Dir("./static")))
	http.HandleFunc("/api/color", setColorHandler)

	log.Println("Server started on :8080")
	log.Fatal(http.ListenAndServe(":8080", nil))
}
