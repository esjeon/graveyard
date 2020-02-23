/* colink.go - embeds a file into as a byte array
 *
 * unlike other famous solutions, this script literally translates a file into
 * a string literal
 */
package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

func main() {
	var err error

	inputPath := os.Args[1]
	symbolPath := os.Args[2]

	fileBytes, err := ioutil.ReadFile(inputPath)
	if err != nil {
		log.Fatal(err)
	}

	outputFile := fmt.Sprintf("colink_%s.go", symbolPath)
	fp, _ := os.Create(outputFile)
	defer fp.Close()

	w := bufio.NewWriter(fp)
	fmt.Fprint(w, "package main\n\n")
	fmt.Fprintf(w, "var %s = []byte(\"", symbolPath)

	for _, b := range fileBytes {
		fmt.Fprintf(w, "\\x%02x", b)
	}
	fmt.Fprint(w, "\")\n")

	w.Flush()
}
