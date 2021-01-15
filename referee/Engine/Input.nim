import std / [algorithm, streams, strformat, strutils]

type
  Input * = ref InputObj
  InputObj = object of StreamObj
    line   *: string
    stream *: Stream

func newInput * (stream: Stream): Input {.inline.} =
  Input(stream: stream)

proc getLine * (input: Input): string {.inline.} =
  while true:
    let line = input.stream.readLine
    if not line.startsWith("WARNING"):
      return line

proc getStr * (input: Input): string =
  while true:
    if input.line == "":
      input.line = input.getLine

    for part in input.line.splitWhitespace:
      input.line.delete(0, part.len)
      return part

proc getInt * (input: Input): int {.inline.} =
  input.getStr.parseInt
