## 1. 작업 목표

현행 물리 질의의 상위 스크립트·native 소유권을 구분하고 검사·낙하·추적 계약을 바꾸지 않는 국소 후보를 검증한다. 기존 게임플레이·운영 배포본은 보존하며 애니메이션 시계/시각 품질은 별도 연구로 유지한다.

## 2. 실제 수행

기존 상세 비용 OFF64b/ON64b C2 ETL을 동일 unwind 정규화 방식으로 다시 읽어 allocation-calling hot leaf부터 Ray/Sphere 바인딩까지의 순서 있는 경로를 선정했다. 새로운 CPU 수집이나 독립 반복이 아니다. 실제 사용6개 PBO 해시와 packed source를 읽어8개 관련 source member의 override를 조사하고 기반 GroundBody 원문 바이트 일치를 확인했다. SDK 선언과 내부 함수의 직접/가상 호출 문맥도 대조했다.

격리 default-OFF 후보는 자체 raw 배열과 고정5개 support offset 배열의 수명만 개체 단위로 연장한다. 출력 points는 매번 새 배열이며 모든 본·발 지지·관절 sweep·fallback 검사 순서와 인자를 보존한다. 기존 FC 캡처 대상은 원래 solver로 위임하고 busy 상태도 원래 경로로 위임한다. 기존 solver로의 문자열 역변환 완전 일치를 확인했다.

후보 PBO를 별도 폴더에 빌드·서명·검증하고, 격리 서버60조건 비교와 기존 평지 하네스의8개체65초 시험을 실행했다. Days Gone 게임은 정상 종료하고 REA 정적 세션은 유지했다. 클라이언트는 실행 전에 안내하고 Computer Use 스킬로 전경·로딩을 확인했다. 영상은 새로 생성하지 않았다.

## 3. 측정 결과

### 기존 CPU 표본 경로 재분석

| 실행 C2 | Ray 경계 아래 hot leaf | Sphere 경계 아래 hot leaf | 해당 두 hot leaf 전체 |
|---|---:|---:|---:|
| 기존 OFF64b |917|785|1,706|
| 기존 ON64b |921|756|1,686|

Ray 경로: c32c10→564960→ea8f60→eb4540→eb2e60. Sphere 경로: c345e0→566780→ea23e0→eb4420→eb2920. 표본 수는 호출·할당 횟수 또는 할당기 self-time이 아니다. 해당 hot leaf 중 query 하위1,702/1,677표본을 설명하며 나머지는 별도 보존한다. 두 함수의 실제 unwind body는각3조각·1,329/1,176바이트다. 첫24바이트만 읽고 전체 함수라고 판단하지 않았다.

### 새 후보 검증

- 격리 서버60조건: 성공40·거부20. 기준/후보 반환·lift·reason·접촉점 일치. 이전 출력 보존, 다른 body 결과, root 불변, busy 강제 시 원래 경로 결과도 일치했다. 실제 재귀 콜백을 발생시킨 시험은 아니다.
-8개체 단일 평지 실행: 두 custom 구간 모두 인식8/8·2m 이상 이동8/8·실제 scratch 적용8/8. 두 구간을 독립2회로 세지 않는다. 전경 frame 비율1.0, 시험 완료·정상 프로세스 종료·강제 종료0·crash log0. 이전 retirement 계약 전체를 사용한 시험은 아니다.
- 새 p50/p95/p99·시각적 포즈 cadence·64개체·수직 등반/낙하 회귀는 미측정/미실행이다. 이번 평지8명으로 해당 gate를 통과시키지 않는다.

후보 적용 확인 TSV는13,088행으로 의도보다 자주 기록됐다. 원시 기록은 보존하며 이 pilot를 성능 비교에 사용하지 않는다. 다음 네 성능 시험에는 동일한 단계별 희소 관찰을 넣었고, 그 관찰 방식 자체의 실제 실행은 아직 없다.

## 4. 원인 분석

확인: 반복 고비용 범위는 스크립트 raw 배열 자체가 아니라 native Ray/Sphere 바인딩의 하위 경로에 있다. SDK RayCastBullet/SphereCastBullet 선언에는 호출자가 내부 후보/결과 버퍼를 제공하는 인자가 없다. 따라서 이번 scratch 후보로 native 내부 컨테이너를 재사용한다고 주장할 수 없다. 질의 내부 자료형·malloc/free 실제 호출 횟수·바이트·GC·self-time은 여전히 미확인이다.

확인: FCExecuteSolve는 FCOn/FCEligible일 때 선택되는 별도 캡처 solver이며 일반 경로는 base로 위임한다. 상세 계측은 SolveContacts/Drive/Displace를 감싸며 native 질의를 대체하지 않는다. 후보도 이 분기와 fresh output 수명을 유지한다.

가설: 자체 반복 배열 생성 감소가 VM/컨테이너 비용 일부를 줄일 수 있다. 추가 wrapper와 guard가 이득을 상쇄할 수도 있고 효과가 아주 작을 수 있다. 이를 주요 native 병목 해결로 선전하지 않는다. 동일 입력42쌍 불일치를 무시한 접촉 결과 캐시나 검사 삭제는 도입하지 않았다.

기존 애니메이션 평균 시간 손실·최종 화면 부드러움의 원인은 이번 후보로 해결되지 않았다. CPU와 영상·Present의 판정은 계속 분리한다.

## 5. 검증 수준

소유권·native 표본 경로: 부분 진단 성공. 후보 실제 엔진60조건 계약: 진단 성공, 단일 서버 실행. 평지8명 추적: 범위 한정 기능 통과, 독립1실행. 반복 성능·8/64 수직 회귀·시각 품질·최종 수용·20ms p95: 미완료다. 전체 문제 해결 골은 active다.

## 6. 변경 사항

새 후보 nf_contact_scratch SHA256=24BC69191DCFB8FBAD788D8BFD33D0F933F1B47A386641E9AB27BCDF63843B3D. packed script/member 동등성·자동 서명·서명 검증 성공. 기존6PBO도 실행 전 검증했다. 개인키는 배포에 포함하지 않았다. 실험용 후보이며 Steam·운영 서버·기존 배포 PBO에는 적용하지 않았다.

추가 파일은 query-owner-20261102 안의 호출 소유권/재분석 JSON, stage_candidate.py, scratch-candidate source/build/mission/launcher/분석 및 보고서다. 기존 source/PBO/원시 로그는 덮어쓰지 않았다. 게임 원본 EXE·native 시계·duration·LOD·14클립 자산은 변경하지 않았다.

## 7. 원시 증거

기존 detailed/off64b와on64b C2 ETL 해시를 이전 파생본과 대조한다. owner-audit.json은6PBO/member 소유권과 SDK 선언, dayz-*/cpu-2-query-paths.json은 순서 있는 표본 경로, allocation-canonical-fragments.json은 chained unwind 조각을 기록한다.

새 실행은 query-owner-20261102/scratch-candidate의 contract-server와flat8이다. contract-result/60행TSV, build-v3/packed-members-proof-v3/signature-v3, flat8 launch/mission-used/monitor-closed/host/custom-agents/client-frames/scratch-active/스크립트·RPT 및 batch-results.json을 연결한다. 최초 로그 빈도 문제와 알려진 진단 플러그인·종료 리소스 경고를 보존한다. 공개 자료는 집계·선정 경로·자체 변경 요약이며 전체 로그·프로세스 정보·설치 자산·키는 제외한다.

## 8. 잔여 문제

64 평지 OFF→ON→ON→OFF는 같은7PBO·카메라·플레이어 입력·희소 관찰을 사용하고 detailed observer는 네 실행 모두OFF로 고정했다. scratch profile flag만 바뀐다. 실제 옵션 적용·목표 인식/이동·active 비율과 Present/CPU busy·프로세스 CPU를 함께 검증한다. 기존 빈번한 pilot 로그를 비교 기준으로 쓰지 않는다.

사전 의미 있는 성능 후보 기준은 두 쌍 모두 개선 방향, median p95 최소5% 및2ms 감소다. 모든 수치·변동과 기능 위험을 보고하며 단일 유리한 실행으로 채택하지 않는다. 효과가 작거나 기능이 깨지면 후보를 기각하고 범위를 확대해 성공을 만들지 않는다. 채택에는 별도8→64 수직·정상 낙하 복구 회귀가 필요하다.

실제 표시 시간에는 Windows ETW 관리자 승인이 필요하여 사용자의 직접 UAC 승인을 요청했다. 바탕화면의 ‘DayZ NPC scratch64 표시 계측 (관리자 승인)’은 지정4개 프로필만215초씩 측정하는 bounded helper다. 설치·권한 그룹 변경·CPU ETL·입력 추적은 없다. 승인 전에는 이 네 성능 시험을 시작하지 않는다. 아직 승인/외부 도구 실행이 확인되지 않았다.

로컬 checkpoint SHA-256: d9596f462ee3e1bd172ddcc555c96e65353d52890720149409eaaf35dc204455
