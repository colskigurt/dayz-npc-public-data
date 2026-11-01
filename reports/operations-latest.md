# 런처 실행 분기와 native 계측 없는 월드 이탈 재현

작성: 2026-11-01T16:55:59.097959+09:00

## 1. 작업 목표

네트워크 애니메이션 시험의 월드 이탈을 계측기 문제와 실행 환경 문제로 분리한다. 설치 런처의 지원 실행 분기를 확인하고 native 관찰기 없는 기준 실행을 수행한다. NPC 시간 손실·표시 연구와 기존 기능은 보존한다.

## 2. 실제 수행

REA의 managed 정적 분석으로 설치 DayZLauncher 1.6.164.64의 타입·메서드·호출·문자열을 보존했다. SHA 592ceb0cc675e734fb89eb6db22694ae87d26299cae501ce856db268d03d821b, MVID d0a02078-db97-445f-a4d7-f2bff607bebd에만 해당한다. 선택 CIL은 ReflectionOnly 메타데이터와 명령 바이트로 대조했으며 대상 메서드를 실행하지 않았다. 세 메서드의 원시 IL 해시와 명령 수가 REA 근거와 일치했다. 첫 디코더 실행의 실행 정책·의존성·opcode prefix 오류는 조사 도구의 준비 실패이며 NPC 변경이 아니다. 현재 디코더는 성공했다. PowerShell 실행 정책은 해당 프로세스 인자로만 지정했고 영구 정책은 변경하지 않았다.

ReRunGameWithLastParametersWithoutBattlEye는 RunGame(lastParameters,false,0)을 호출한다. RunGame의 false 분기는 64bit 게임 실행 파일을, true는 BattlEye 실행 파일을 선택하고 BE용 인자와 별도 모니터링을 구성한다. 파서의 -useBE는 실제 bool 설정에 연결된다. 임의의 -noBE 인자는 만들지 않았다. 다만 UI·Steam 설정 전체의 의미나 실행 후 상태를 이 정적 사실로 확정하지 않는다.

계측 없는 기준 실행을 준비했다. 첫 준비 스크립트는 문자열 치환 실패로 Steam에 잘못된 인자를 전달했고 클라이언트가 생성되지 않았다. 실패 스크립트를 보존하고 별도 명시적 직접 실행 스크립트로 수정했다. 같은 격리 서버에 실제 접속해 고정 run·루트 이동 OFF·물리 1파트 조건으로 실행했다. Python Frida/네이티브 관찰기는 전혀 연결하지 않았다. 컴퓨터 유즈로 실제 월드와 한 모델 파트 표시를 확인했다.

## 3. 측정 결과

성공적으로 시작된 기준 실행은 1회다. 서버 pose 2151행, DONE 완료, 클라이언트 종료 marker 확보, 클라이언트 강제 종료 없음, crash log 0개다. 서버는 DONE 후 소유 프로세스만 정지했다. NPC retirement 계약 검증은 아니다.

NPC READY에서 disconnect까지 37.331초이며 서버 DONE보다 7.386초 앞선다. authFailed=false, alive=true, health=100, logoutTime=15다. 이는 해당 이벤트 값일 뿐 인증 전반의 정상 증명이 아니다.

게임 내 fresh Process 조회는 121개 모듈 중 BEClient_x64.dll을 확인했고 RPT에는 BattlEye client updated to version:249가 있다. 초기 반복 module snapshot의 52개 목록은 Process 객체 캐시였으므로 DLL 부재 근거로 사용할 수 없다. 다른 프로세스 검색에서는 해당 이름의 BE 실행기/서비스를 발견하지 않았으나 엔진 내부 활성 상태까지 증명하지 않는다. 마지막 오류 화면은 client 종료 후라 저장하지 못했고 이번 disconnect 코드는 미확인이다.

새 8/64 회귀, frame-time p50/p95/p99, GPU/실제 표시 시각, CPU self-time, 할당/GC 및 시각 품질은 측정하지 않았다.20ms p95는 목표다.

## 4. 원인 분석

확인된 사실: 네이티브 관찰기를 붙이지 않은 실행도 예정 서버 종료 전에 월드를 떠났다. 따라서 모든 이탈에 native attachment가 필요하다는 설명은 반증됐다. 이전 hook ON 시험에서 포착한 BattlEye restart-required 화면과 이번 이탈이 같은 원인인지는 아직 모른다.

추정: 실행 환경이나 플랫폼 연결 상태가 공통 원인일 수 있지만 현재는 후보일 뿐이다. BEClient DLL 로드·갱신 로그만으로 active/inactive를 확정하거나 바로 DLL을 제거·변경해서는 안 된다. 서버 cfg BattlEye=0 또는 런처의 직접 실행 분기만으로 client 비활성 검증을 대신할 수 없다.

도구 한계: cached module snapshot의 부재 판단은 기각했다. fresh 조회로 수정했다. CU의 종료 시점 화면 확인은 실패했으므로 오류 코드나 시각 품질 판정에 사용하지 않는다. 원래 오프라인 mode0의50ms 누적기 reset 손실 근거를 이번 연결 이슈로 대체하지 않는다.

## 5. 검증 수준

런처 분기 식별과 원시 IL 대조는 진단 성공이다. 계측 없는 이탈 재현은 1회 관찰이며 반복 검증·해결 성공이 아니다. client BattlEye 비활성 상태, 원인 동일성, 전체 기능 회귀, 시각 품질과 성능의 최종 수용은 미완료다.

## 6. 변경 사항

기존 research 하위에 작은 정적 분석 파일·격리 launch/mission/profile·집계만 추가했다. 게임 코드·PBO·ANM·9350 후보·운영 서버는 변경하지 않았다. 기존3개 PBO 해시와 서명은 시험 전 모두 검증 성공했다. 새 PBO가 없어 신규 서명 대상은 없다. 서비스·드라이버·영구 보안 설정·anti-cheat 바이너리를 변경하지 않았다. 새 영상·대용량 dump·도구 설치는 없다.

## 7. 원시 증거

분석 ID launcher-test-mode-20261101. rea-managed-evidence/rea-caller-evidence는 선택 정적 Evidence 원본이다. decoded-cil/decoded-entry-cil은 별도 ReflectionOnly 파생본이다. selected-evidence는 공개용 선택 범위를 명시한다. direct-no-observer에는 처음 실패한 준비본과 성공한 명시적 실행본, 서버 미션·로그·pose·ingame-modules·게임 내 화면을 보존한다. 해시와 JSON·파일 안정성을 확인한 후 기존 체크포인트 체계로 저장했다. 전체 원시 로그·화면·프로세스 정보·로컬 절대 경로·키·게임 자산은 공개하지 않는다.

## 8. 잔여 문제

다음 최소 시험은 서버 disconnect marker를 기다리는 CU 오류 화면 저장을 먼저 연결한 뒤, native 관찰기 없는 기준 실행의 실제 메시지를 확인하는 것이다. 같은 native 부착 시험을 반복하지 않는다. 메시지가 다르면 원인을 분리하고 같으면 지원 실행 설정과 플랫폼 연결 경로를 더 확인한다. client 상태가 검증되지 않으면 네트워크 애니메이션 계측은 보류하고 확보된 오프라인 시계·표시/64개체 비용 분석을 이어간다. 새로운 시간 배율·보간·충돌 완화 구현은 보류한다.

선정 공개 본문. 로컬 보고서 SHA-256: `9da62626cfafa746662a3ff565a86ed97ca9990232141043b6332792011201c4`. 원시 전체가 아니다.
