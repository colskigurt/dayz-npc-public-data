# 수집기 종료코드 계약 수정 및 중복 검토

## 1. 작업 목표

현재64개체 CPU/물리 비용 최적화를 재개하기 위해 기존 Drive/Displace 결합 검토와 최신 실행 근거의 중복을 확인하고,다음 실제 비용 비교를 막는 수집기 종료코드 누락을 수정한다. 이 운영 수정 자체를 NPC 성능 개선으로 보고하지 않는다.

## 2. 실제 수행

기존 COMBINED_PROPOSAL_FEASIBILITY-20261105-191155 보고서와 원본 Ground/Crowd/Probe 소스의 기존 SHA-256 네 개를 대조했다. 기존 결론에 새 안전성 근거가 없으므로 결합 솔버·무조건 결과 캐시·이미 존재하는 zero-displacement guard를 다시 구현/시험하지 않았다.

현재 통합 실행기 tools/npc-test-launcher.ps1의 수정 전 파일을 보존하고 PresentMon Start-Process 직후 실제 Handle을 보유하도록 변경했다. 유한 대기 성공 뒤 WaitForExit를 완료하고 ExitCode를 한 번 읽어 closed.json과 오류 분기에서 같은 값을 사용한다. null/nonzero/CSV없음 거부 조건은 유지한다. 기존 미션·PBO·현재 설정 포인터·과거 실행 기록은 변경하지 않았다.

실제 수정된 실행기에서 수집 블록을 추출해 정상/비정상 자식 프로세스와 CSV 유무3조건을 시험했다. CSV는 시험용 가짜 텍스트이며 성능자료가 아니다. 설치된 서명 고정 PresentMon의 --help만 실행해 실제 벤더 프로세스에서도 종료코드를 읽었다. ETW·게임·관리자 실행은 없었다.

## 3. 측정 결과

| 종료/파일 조건 | 읽은 코드 | gate | 판정 |
|---|---:|---|---|
| 자식 exit0 / 시험CSV 있음 |0|통과|기대 일치|
| 자식 exit7 / 시험CSV 있음 |7|거부|기대 일치|
| 자식 exit0 / CSV 없음 |0|거부|기대 일치|
| 실제 PresentMon --help |1|계측 아님|정수 종료코드 확보|

PowerShell AST 오류0. 실제 수정 블록 SHA `CF053193BC34E8CB7EFDDF00D73FA0F5E0194FF7FF5AA6179EECED74A8327E08`. 신규 NPC 실험0,ETW0,8/64 기능·Present p50/p95/p99·CPU Busy·영상 품질은 이번 미측정이다. 실제 ETW 수집의 종료·권한·CSV 전 구간 검증은 다음 실행에 남는다. help의 종료1을 정상 계측으로 수용하지 않는다.

## 4. 원인 분석

확인: 기존 코드가 프로세스 시작 직후 핸들을 보유하지 않고 종료 후 ExitCode를 여러 번 조회했다. 이전 수동8/64 실행에서는 CSV가 있으면서 exitCode=null이 기록됐다. 핸들 보유와 단일 조회를 사용한 이번3조건에서는 0/7/0을 정확히 구분했다. 추정: 누락 원인은 Process 객체의 핸들/종료 관찰 수명에 관련됐을 가능성이 높다. 과거 실행을 재현하지 않았으므로 정확한 과거 인과를 확정하지 않는다.

기존 Drive→Displace는 다른 anchor/heading/minimumLift를 사용하고 첫 성공의 Contacts가 두 번째 swept trajectory의 출발점이다. 단일 endpoint solve로 바꾸면 중간 경로와 부분 성공 rollback이 달라진다. 기존42쌍 입력 완전일치0을 재사용하며 근거없는 병합은 보류한다. 이는 모든 통합 컨트롤러가 불가능하다는 뜻이 아니다.

직전 정적 Bounds 후보는 metadata Update/Apply에서 동등했으나 실제8에서cache hit0이었다. 함수49~53% 감소를 전원생존64 개선으로 옮길 수 없어 주력에서 보류했다. 이 판단을 번복할 새 근거는 없다.

## 5. 검증 수준

기존 호출소유 검토 재사용: 진단 성공,새 실험 아님. 수집기 종료코드 분기:3조건 회귀 통과. 실제 PresentMon help: 종료값 관찰 성공. 실제 ETW 계측·NPC 기능·시각·전체 성능·최종수용: 이번 미실행/미완료. 과거 null코드를0으로 바꾸거나 과거 수집기 오류를 성공으로 소급하지 않았다.

## 6. 변경 사항

통합 실행기 수정 SHA `CA975CF4AC9FF017BC3D8A2C9ECEB60B450DC7AEAD32CA0CD693AD388313B195`,수정전 `12184964A7139EDBDB34BD3AAD6258C35634ED2A8151C9772FC0B66B2CE20882`. 수정 전 원문·patch receipt·시험3조건·원시 stdout/stderr·벤더 help 출력을 프로젝트 안에 보존했다. 새 바탕화면 실행기/관리 체계 없음. 게임 코드·PBO·모델·서명키 변경 없음,PBO 재서명 대상 없음. 기존 날짜별 수집기 스크립트는 과거 실행 재현용으로 보존하며 자동 동기화하지 않았다. 앞으로 현재 통합 실행기만 사용한다.

## 7. 원시 증거

실행 식별 collector-exit-contract1. test-results.json,patch.json,각조건 closed/tool-started/stdout/stderr,실제 벤더 help 출력,기존 combined feasibility 및 integrity를 연결한다. 공개 패킷은 비민감 시험 결과와 실제 수집 블록만 포함하며 개인 경로·프로세스 식별·전체 과거 로그·키는 공개하지 않는다. 기존 기록의 과거publicUpdated=false는 당시 상태로 보존하며 이번 게시를 소급 기록하지 않는다.

## 8. 잔여 문제

다음 실제 비용 비교는 수정된 단일 실행기와 새 실행 ID를 사용해야 한다. ETW 관리자 승인은 기존 도우미 흐름을 재사용하며 승인 대기/실패를 성공으로 취급하지 않는다. 기존5파트64개체의 같은 평지/카메라/목표 조건에서 반복 가능한 baseline을 확보하고,그 baseline에 대해 실제 수정 가능한 다음 단일 후보를 고른다. 이미 실패한rank·단순Drive/Displace병합·hit0 Bounds를 다시 시험하지 않는다. 기능적 실패를 성능 작업 전체의 기능 개발로 전환하지 않는다. 20ms p95는 목표이며 미달성이다.
