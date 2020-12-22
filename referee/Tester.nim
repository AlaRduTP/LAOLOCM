import std / [osproc, parseopt, strformat, strutils]
import Engine / Input
import Research / IOHelpers

when isMainModule:
  proc main(): void =
    var referee = "java --add-opens java.base/java.lang=ALL-UNNAMED -jar referee/LoCM.jar"
    var baseline: string
    var agent: string

    var replays = false
    var threads = countProcessors()
    var plain = true
    var games = 0
    var sofar = 0
    var wins1 = 0
    var wins2 = 0

    for kind, key, value in getOpt():
      if key == "games":   games   = value.parseInt
      if key == "plain":   plain   = value.parseBool
      if key == "baseline": baseline = value
      if key == "agent": agent = value
      if key == "referee": referee = value
      if key == "replays": replays = value.parseBool
      if key == "threads": threads = value.parseInt

    if not plain:
      echo &"{stamp()} Referee: {referee}"
      echo &"{stamp()} Player1: {baseline}"
      echo &"{stamp()} Player2: {agent}"
      echo &"{stamp()} Games:   {games}"
      echo &"{stamp()} Replays: {replays}"
      echo &"{stamp()} Threads: {threads}"

    var commands = newSeq[string](games)
    for index in commands.low .. commands.high:
      commands[index] = &"""{referee} -p1 "{baseline}" -p2 "{agent}" -d "draftChoicesSeed=$RANDOM seed=$RANDOM shufflePlayer0Seed=$RANDOM shufflePlayer1Seed=$RANDOM""""

    discard execProcesses(
      cmds = commands,
      n = threads,
      options = {poStdErrToStdOut},
      afterRunEvent = proc (id: int; process: Process) =
        var output = process.outputStream.newInput
        # discard the warning line
        discard output.getLine
        let score1 = output.getInt
        let score2 = output.getInt
        let error = score1 < 0 or score2 < 0

        sofar += 1
        wins1 += score1.max(0)
        wins2 += score2.max(0)

        # In this mode only final results are reported.
        if plain:
          return

        if score1 < 0 or score2 < 0:
          echo &"{stamp()} End of game {sofar:>3}: ERRORED {score1} {score2}"
        else:
          let total = wins1 + wins2
          let index = total - 1
          let proc1 = 100 * wins1 / total
          let proc2 = 100 * wins2 / total
          echo &"{stamp()} End of game {sofar:>3}: {proc1:6.2f}% {proc2:6.2f}%"

        if error or replays:
          echo &"{stamp()} Replay game {sofar:>3}: {commands[sofar - 1]} -s"
    )

    if plain:
      # echo &"{wins1} {wins2}"
      if wins1 > 0 or wins2 > 0:
        echo &"{wins2 / (wins1 + wins2)}"
      else:
        echo "0"

  main()
