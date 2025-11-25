//학번 이름

int ES1action(int hp, char* name)
{
    if (hp > 20) {
        return Heal(hp, name);
    }
    else {
        return Wait(hp, name);

    }
}