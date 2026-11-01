# PowerShell 판정 수정과 진단 연결 기준 시험: 검증 범위 명확화

작성: 2026-11-01T17:16:20.841625+09:00

## 1. 작업 목표

진단 서버를 NPC 생성 전에 멈추게 한 실제 PowerShell 하네스 판정을 검증하고, 지원 진단 환경에서 계측 없는 연결 기준 시험을 완료한다. 기존 NPC 파이프라인·일반 실행 파일의 시계 근거와64개체 비용 연구는 보존한다.

## 2. 실제 수행

이전 종료 로그와 동일한 JSON·판정식을 PowerShell5.1과7에서 각각 실행했다.5.1에서는 @(ConvertFrom-Json pipeline)이 내부 Object[]를 감싼1개 배열을 만들고7에서는8개 문자열이 된다. 따라서 이전 v2의 expectedLines.Count==8 조건은 실제 실행 환경5.1에서 항상 false였다. 기존 완료 로그 replay는7에서 실행되어 true였다. 배열을 InputObject로 읽고 명시적으로 string[]로 변환하도록 수정했다.

실제5.1에서 clean/complete/duplicate/partial/timeout/unexpected/altered/compile/partial-last-line/unknown-last-line/이전 실제 완료 로그 등11개 검사를 통과했다. canonical script_*.log만 스크립트 오류 원본으로 판정해 RPT/error.log의 복사를 이중 집계하지 않는다. 검토된8줄 경고와 정확 일치하는 prefix만 최대2초 보류하며 다른 오류·compile/role failure·crash는 중단한다. 거부 시 실제 snapshot과 이유를 저장하도록 했다.

수정 후 설치 DayZDiag client/server로 기존 물리1파트·고정run·루트 이동OFF 조건의45초 미션을1회 실행했다. 새 native 관찰기는 연결하지 않았다. CU로 모델 파트가 보이는 실제 월드를 확인하고, Process.Refresh 뒤 모듈을8초 간격으로 조회했다.

## 3. 측정 결과

PowerShell5.1.26100.9711: wrappedCount1, 첫 원소 Object[], oldCountGate=false. PowerShell7.6.5: wrappedCount8, 첫 원소 String, oldCountGate=true. 명시적 변환은 두 환경 모두8줄·기존 실제 로그24줄의 반복과 일치했다.5.1의 guard 검사11/11 통과다.

진단 baseline: 미션 DONE 완료, READY→DONE host wall 44.712초, pose 2151행, 예정 종료 전 disconnect0, BattlEye kick0, crash log0개다. 미션의45초는 simulation 조건이며 실제 host wall과 구분한다. client 종료 marker 확보·강제 종료 없음, server는 DONE 후 소유 프로세스만 정지했다. NPC retirement 계약 검증은 아니다.

fresh 모듈 표본9회에서 BEClient/BattlEye 이름 모듈0개, client BE 갱신 로그0개다. 이는 해당 시점의 관찰 범위이며 모든 엔진 내부 security 상태의 완전 증명은 아니다. 기본 PluginItemDiagnostic 경고8줄은 그대로 남았고 최종 guardDecision은 known-warning·완결1블록·partial=false다. 경고 없는 실행으로 보고하지 않는다.

8/64개체 회귀, client native mode/graph clock/본 행렬, frame-time p50/p95/p99, CPU self-time·GPU/display·할당/GC는 이번에 측정하지 않았다. 한 장의 화면으로 시각 부드러움을 판정하지 않는다.20ms p95는 목표다.

## 4. 원인 분석

확인된 원인: 하네스의5.1/7 JSON 배열 형태 차이가 이전 live false/오프라인 true를 재현한다. NPC 알고리즘이나 애니메이션 실패가 아니었다. 이전 보고서에서 후보로 둔 쓰기 중 snapshot 가설은 이 불일치 설명에 필수적이지 않으며, 실제로 발생했다고 입증된 적도 없다. 동적 로그는 부분 기록 가능성이 있어 bounded pending 검사를 따로 유지했다.

확인된 사실: 수정한 하네스에서 진단 실행 경로는 예정된 미션 끝까지 연결을 유지했다. 일반 실행에서 계측 없이도 발생한0x000400F0/BattlEye restart 경고는 여전히 별도 미해결 항목이다. 실행 파일·조건이 달라졌으므로 이번 성공을 일반 exe의 연결 수정·성능/애니메이션 개선으로 소급하지 않는다.

미확인: 진단 client의 native mode·retained pose·시계·실제 표시 cadence는 다음 계측 대상이다. 기존 일반 exe 주소를 그대로 쓰지 않는다. 이 환경에서 지원되는 관찰 경로를 확보한 것과 게임에 배포할 해결책은 별개다.

## 5. 검증 수준

PowerShell 배열 원인 재현·guard11개 검사는 진단 성공이다. 진단 연결 기준 실행은1회 통과이며 반복 검증 완료가 아니다. 일반 client 연결 해결·전체8/64 기능 회귀·시각 품질·20ms p95 및 최종 수용은 미완료다.

## 6. 변경 사항

격리 research 내부 Get-ScriptLogDecision/Test/Inspect-JsonArrayShape, fixture launcher·expected warning·CU 저장만 추가했다. 게임 코드·PBO·ANM·9350 후보·운영 서버는 변경하지 않았다. 기존3개 PBO 해시·서명 검증은 성공했다. 새 PBO가 없어 신규 서명 대상은 없다. 서비스/드라이버/영구 정책/anti-cheat 바이너리 변경·새 설치·영상·대용량dump는 없다.

진단 exe SHA A500BE804FFED5CDE369337AEB6AB477EF5EE0B5015764BE2B1953C85EC3060D를 고정했다. 일반 C670/EE1A의 결과와 섞지 않는다. 판정기 변경은 시험 중단 판단에 한정되며 게임 내 NPC 상태 전이를 수정하지 않는다.

## 7. 원시 증거

분석 ID diag-guard-20261101-1710. array-shape-ps51/ps7, powershell-guard-tests, Get/Test/Inspect PS원문, runtime launch/profile/role-pose/ui-observation을 보존한다. 실제 관찰 이미지 SHA 2872b717da6369c97a0d1938b263a687a0977c7cf0cdd3200f6d066ee7172213이다. 이전 경고·실패 로그는 원래 폴더에 그대로 두고 참조했다. 합성 검사는 판정 함수의 오류 거부 결과를 확인했다. launcher의 실제 거부 snapshot 파일 저장 분기는 이번 실행에서 사용되지 않아 파일 생성까지 검증된 것은 아니다.

공개본은 선택 판정기 함수·테스트 결과·집계·식별자 없는 이벤트·이미지 해시만 제공한다. 전체 raw 로그·이미지·CLI·프로세스 정보·로컬 절대 경로·사용자 식별자·게임 자산·개인키는 제외한다. 종료·JSON·기존 해시·파일 안정성을 확인한 뒤 기존 체크포인트 체계에 저장했다.

## 8. 잔여 문제

다음은 A500 진단 exe의 native 애니메이션 경로를 해당 빌드의 정적 근거로 매핑한 뒤, 이 검증된 진단 환경에서 bounded 읽기 전용 관찰을1회 진행하는 것이다. 일반 exe 주소·구조를 검증 없이 재사용하지 않는다. 확보되면 동기 시각·서버/API 본·client retained pose·표시를 구분한다.

진단 기반 결과는 일반 exe의 별도 검증이 필요하다. 기존 mode0 누적기 손실과 원본/변환 대조 결과·64개체 회귀/비용은 유지한다. 새 보간·시간 배율·충돌 생략을 추측으로 적용하지 않는다.


검증 범위 정정: 직전 체크포인트의 snapshot 분기 검증 표현을 판정 함수 검사 범위로 한정했다. 원래 보고서와 원시 자료는 보존했다. 추가 게임 실행은 없다.

선정 공개 본문. 로컬 보고서 SHA-256: `b533f8d72d962941f6a57338aaa472956d8320d3ca3854728cf8241d5b64b93a`. 원시 전체가 아니다.
