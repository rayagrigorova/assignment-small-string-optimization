# Small string optimization

### Имплементация на стринг, в който при достатъчно малки стрингове се прилага SSO.

Класът представлява union, съдържащ инстанции на две структури. Първата структура longStr е низ с произволна дължина - има pointer към данните,
капацитет и дължина. shortStr е статичен char масив с големина 15 и променлива за дължината на стринга. 