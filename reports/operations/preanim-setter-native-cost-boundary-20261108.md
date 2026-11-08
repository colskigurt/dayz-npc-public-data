# 그래프setter 네이티브본문 경계 — 전체비용미확정

## 1. 작업 목표
유효ON재측정의창유지가능여부를기다리는동안,기존바이너리/표본으로setter의직접네이티브비용범위를좁힌다.새게임·후킹·전체엔진분석없음.
## 2. 실제 수행
기존기록에서같은분석을찾고REA세션상태를확인했다(열린대상없음).기존해시검증PE/등록디코더·Capstone을재사용해현재서버의PreAnim_SetInt/Bool등록4개를추적했다.등록함수최대64KiB·handler4096byte한도,원문불변.Ghidracold분석/설치/설정변경없음.기존CPU요약의보존범위도확인했다.
## 3. 측정 결과
현재서버SHA987fbc47...aebba805.RVA a22a10/a22960의setter본문각6명령,a9fca0/a9fbe0각8명령.두계열모두직접값저장후ret이며call명령/하위callee없음.두번째등록문맥에AnimCommandBase문자열,SDK에DayZAnimalCommandScript extends AnimCommandBase를확인했다.실제live디스패치계열은미관측.
기존CPU요약:10.847172초·가중표본8101,스택누락2268.함수목록은leaf/inclusive각상위100개만보존돼setter가목록에없어도0표본/0비용으로판정할수없다.
새CPU/Present p50/p95/p99·8/64실행·시각측정0.
## 4. 원인 분석
확인:선정setter본문안에서그래프평가·물리질의·할당함수를호출한다는설명은바이트흐름과맞지않는다.단순인덱스저장과한계열의활성바이트검사다.
미확정:VM디스패치·인자전달·캐시분기의전체비용과실제CPU절감효과.짧은본문이낮은전체경계비용을증명하지않으며opcode수로ms를추정하지않는다.A56650의AnimationVariableChangeLog표본범위를이setter비용으로명명할근거도없다.
## 5. 검증 수준
선정등록·바이트흐름부분정적진단성공.런타임함수소유자/정확self-time/64개선미확정.전체기능·시각·성능미수용,20ms p95목표유지.
## 6. 변경 사항
기존cpu-owner1아래읽기전용검사기와파생JSON만추가.게임코드·PBO·자산·분석설정미변경.새서명대상없음.직접E8호출자조회0건은간접등록의부재증명이아니다.
## 7. 원시 증거
preanim-setter-bindings1.json,preanim-setter-registration-owners1.json,preanim-registration-direct-callers1.json,preanim-setter-cost-boundary1.json,기존capture-1/cpu-1-samples-target-v1.json.각함수원문해시·현재바이너리해시·등록문맥을연결했다.기존ETL/요약을변경하거나새로수집하지않았다.
## 8. 잔여 문제
유효한전체CPU효과는아직판정불가.ON교체프로필만준비했고창유지가능답변전에는실행하지않는다.이번정적결과로성능성공/효과없음을대체하지않는다.다음핵심산출물은조건을지킨ON교체측정이다.
