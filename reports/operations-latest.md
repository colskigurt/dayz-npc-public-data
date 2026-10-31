# 실제 표시 계측 준비와 ETW 권한 제한 확인

작성: 2026-10-31T17:29:03.124082+09:00

## 1. 작업 목표

CPU 포즈와 실제 표시 계층을 구분하기 위해 ETW 기반 Present/표시 간격 측정 경로를 준비한다. 게임 전체 표시 주기와 NPC 포즈 품질을 서로 대체하지 않는다.

## 2. 실제 수행

기존 presentation-motion 도구는 world/root 변환 분석이며 실제 Present/GPU 측정이 아님을 확인했다. 로컬에는 PresentMon이 없었고 Windows wpr/logman만 있었다. 공식 GameTechDev/PresentMon v2.6.0의 독립 x64 실행 파일980320바이트를 기존 프로젝트 tools/vendor/PresentMon/2.6.0에 보관했다. 설치형169MB패키지·드라이버·서비스는 설치하지 않았다. GitHub release SHA-256과 다운로드 hash가 일치했고 Intel Authenticode서명이Valid였다. MIT license와 고정버전 문서를 연결한다.

게임 없이 소유 PowerShell PID만 대상으로 고유 이름의2초 ETW preflight를 실행했다. 기존세션중단·관리자재시작옵션을 사용하지 않았다. 도구는 즉시접근거부로 종료됐다. 이후 사용자가직접UAC를승인할 수 있는 일회용 관리자스크립트와 바탕화면바로가기를 준비하고 구문을검사했다. 바로가기를 자동실행하거나UAC를조작하지 않았다. 사용자응답대기중이며게임은실행하지 않았다.

추후CSV분석기는PID격리·swap-chain분리·NA보존·QPC단위검사를 준비했다. 합성parser검사만 수행했고실제CSV로검증한것은아니다. Windows QPC주파수10000000을기록했다.

## 3. 측정 결과

| 항목 | 결과 |
|---|---|
| 공식 release digest 대조 |일치|
| Authenticode |Valid, Intel Corporation|
| 현재 관리자/Performance Log Users |False/False|
| ETW preflight |exit6, access denied, 종료확인|
| 그래픽CSV 생성 |False|
| 실제 Present/표시 p50/p95/p99 |미측정|
| 관리자스크립트 |구문통과, 미실행|
| parser 합성검사 |6항목통과, 실제측정아님|
| 새게임/PBO/영상 |0/0/0|

preflight대상은그래픽frame을기대하지않는소유PowerShell이었다. frame0자체로실패를판정한것이아니라명시적인ETW access denied를근거로삼았다. 도구확보나parser통과를측정성공으로보고하지않는다.

## 4. 원인 분석

확인:현재실행토큰으로ETW세션을시작할권한이없다.이는NPC코드나GPU동작실패가아니다. 공식도구도관리자또는Performance Log Users권한을요구한다고보고했다. 따라서추가게임을실행해같은실패를반복하지않았다.

미확인:실제Present/표시interval·GPU busy·drop/NA·로그완전성·NPC포즈의화면소비시점이다. PresentMon의MsAnimationError는frame CPU/display delta차이이며감염자본포즈시간오차가아니다. 툴을사용할수있게돼도GPU skinning self-time이나특정NPC포즈표시를직접증명하지않는다.

## 5. 검증 수준

도구출처/다운로드/서명·권한사전점검·parser합성검사:진단성공. 실제그래픽측정·CPU/native와ETW상관·시각품질·64기능/성능·최종수용:미실행/미완료.20ms p95는목표다.현재권한요구는이측정경로의제한이며전체골완료나중단으로표시하지않는다.

## 6. 변경 사항

약1MBportable도구·출처기록·read-only측정/분석준비파일·사용자용바로가기만추가했다.게임Source·미션·PBO·운영서버·기존원시기록·키는보존했다.새PBO/서명대상은없고도구자체디지털서명검증과구분한다.관리자스크립트는해시/서명·허용profile·현재소유PID·B8FB진단빌드를검사한뒤한번30초만실행하도록준비됐다.사용자권한그룹변경·서비스설치·게임실행/종료·기존ETW세션중단은없다.

## 7. 원시 증거

tool-provenance·고정버전문서/help·preflight stdout/stderr/종료record·parser selftest·QPC reference·관리자스크립트와구문/shortcut준비record를연결한다.공개는선정집계와공식출처이며로컬경로·프로세스ID·실행명령·바이너리는노출하지않는다.실제capture파일이없다는제한도보존한다.종료한preflight와고정파일의hash/JSON/안정성을검증해저장한다.

## 8. 잔여 문제

바탕화면 'DayZ NPC 표시 계측 (관리자 승인)'을사용자가실행해UAC를직접승인해야한다.준비완료창이확인된뒤새격리1개체실행의실제준비상태와PID를검증하고30초측정요청을만든다.승인없이권한을우회하거나그룹을바꾸지않는다.실제CSV가생기면PID·swap-chain·QPC·NA·event-loss·컬럼을검증한뒤에만분석한다.관리자승인이어려우면기존자료분석을계속하고실제ETW측정은미확인으로남긴다.

선정공개본문.로컬보고서SHA-256:`328aa8e2a15a4da7dd6ad3a2e4ce30f98cefcb8d790ae260eae83e0d0ff4bb52`.전체raw가아니다.
