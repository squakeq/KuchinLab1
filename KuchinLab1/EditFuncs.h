#pragma once
#include "Header.h"


void EditCS(CSMap& stations);

void Delete(PipeMap& pipes, CSMap& stations);

void ChangePipeStatus(PipeMap& pipes, int pipe_id, bool solo_edit);
