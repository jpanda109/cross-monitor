with open('location.log', 'r') as location_file:
    for line in location_file:
        parts = line.split(' ')
        x = parts[0][2:]
        print('x: ' + x)
        y = parts[1][2:]
        print('y: ' + y)
        screen = '--screen ' + parts[2][7:]
        window = '--window ' + parts[3][7:].rstrip()
        with open('location_parsed.log', 'a') as parsed_file:
            parsed_file.write('xdotool mousemove' +
                              ' ' + x + ' ' + y + ' click 3' + '\n')
